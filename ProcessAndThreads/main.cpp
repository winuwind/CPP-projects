#include <iostream>
#include <cmath>
#include <vector>
#include <pthread.h>
#include <random>
#include <mpi.h>
#include <atomic>
#include <thread>

using namespace std;

int world_size;
int world_rank;
atomic<bool> flag_iteration{true};
atomic<bool> flag_work{true};
atomic<bool> flag_need_tasks{false};
vector<int> tasks;
atomic<int> numberTask{0};
atomic<int> sizeTasks{0};
int num_iteration = 0;
int count_iteration = 100;
int countDoneTasks = 0;
int countReceivingTasks = 0;
int countSendingTasks = 0;

pthread_mutex_t mutex_tasks = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_getter_local = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_getter_global = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_getter_global = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_getter_local = PTHREAD_COND_INITIALIZER;

void* doTask(int count_iterations, volatile int* result){
    *result = 1;
    for(int i = 0; i < count_iterations; i++){
        for(int j = 0; j < i; j++){
            *result += static_cast<int> (sqrt((i + 100) / (j + 1))) * (i - j) - 12;
        }
    }
    countDoneTasks++;
    return nullptr;
}

void control(){
    flag_iteration = true;
    flag_need_tasks = false;

    pthread_mutex_lock(&mutex_getter_global);
    pthread_cond_signal(&cond_getter_global);
    pthread_mutex_unlock(&mutex_getter_global);

    int result, count_iterations;
    pthread_mutex_lock(&mutex_tasks);
    sizeTasks = tasks.size();
    pthread_mutex_unlock(&mutex_tasks);
    numberTask = 0;
    while (flag_iteration){
        if(numberTask < sizeTasks){
            pthread_mutex_lock(&mutex_tasks);
            count_iterations = tasks[numberTask++];
            if(!flag_need_tasks && sizeTasks - numberTask < 10){
                flag_need_tasks = true;

                pthread_mutex_lock(&mutex_getter_local);
                pthread_cond_signal(&cond_getter_local);
                pthread_mutex_unlock(&mutex_getter_local);
            }
            pthread_mutex_unlock(&mutex_tasks);
            doTask(count_iterations, &result);
        }
        else{
            flag_need_tasks = true;

            pthread_mutex_lock(&mutex_getter_local);
            pthread_cond_signal(&cond_getter_local);
            pthread_mutex_unlock(&mutex_getter_local);
        }
    }



    flag_need_tasks = false;
    flag_iteration = false;

    pthread_mutex_lock(&mutex_getter_local);
    pthread_cond_signal(&cond_getter_local);
    pthread_mutex_unlock(&mutex_getter_local);
}

void generateTasks(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 50);

    int count_ = 1200 / world_size;

    tasks.clear();
    for(int i = 0; i < count_; i++){
        tasks.push_back(distrib(gen) * (fabs(world_rank - num_iteration % world_size) + 1) * 25 / (world_size + 1));
    }
}

void sendTasks(){
    int lenAskedTasks = 0;
    MPI_Status status_r;
    int complete_recv = 0;
    while(!complete_recv && flag_work){
        MPI_Iprobe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &complete_recv, &status_r);
        if(complete_recv){
            break;
        }
        else{
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    if(!complete_recv){
        return;
    }
    int count;
    int source = status_r.MPI_SOURCE;
    int tag = status_r.MPI_TAG;
    complete_recv = 0;
    MPI_Get_count(&status_r, MPI_INT, &count);
    int* buffer = new int[count];
    MPI_Recv(buffer, count, MPI_INT, source, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    if(!(flag_work && flag_iteration)){
        delete[] buffer;
        int x = -1;
        MPI_Request request;
        MPI_Isend(&x, 1, MPI_INT, source, tag + 1, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, MPI_STATUS_IGNORE);
        return;
    }
    if(count != 1){
        cout << "ERROR WHEN RECV" << endl;
        cout << "SIZE DATA = " << count << endl;
        MPI_Abort(MPI_COMM_WORLD, 6);
    }
    lenAskedTasks = buffer[0];
    delete[] buffer;

    int countFreeTasks = sizeTasks - numberTask;
    MPI_Request request;
    if(countFreeTasks > 60){
        int sendingCount = static_cast<int>(fmin(countFreeTasks - 60, lenAskedTasks));
        countSendingTasks += sendingCount;
        int* sendingTasks = new int[sendingCount];
        for(int i = 0; i < sendingCount && numberTask + i < sizeTasks; i++){
            sendingTasks[i] = tasks[numberTask++];
        }
        MPI_Isend(sendingTasks, sendingCount, MPI_INT, source, tag + 1, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, MPI_STATUS_IGNORE);
        delete[] sendingTasks;
    }
    else{
        countFreeTasks = 0;
        MPI_Isend(&countFreeTasks, 1, MPI_INT, source, tag + 1, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, MPI_STATUS_IGNORE);
    }
    pthread_mutex_unlock(&mutex_tasks);
}

void* senderThread(void*){
    while (flag_work){
        sendTasks();
    }
    return nullptr;
}

void getTasks(){
    pthread_mutex_lock(&mutex_getter_local);
    while(!flag_need_tasks && flag_iteration && flag_work){
        pthread_cond_wait(&cond_getter_local, &mutex_getter_local);
    }
    pthread_mutex_unlock(&mutex_getter_local);
    if(!flag_iteration || !flag_work){
        return;
    }
    int count_ask = 50 / world_size;
    for(int source = 0; source < world_size; source++){
        if(world_rank == source){
            continue;
        }
        MPI_Request request;
        MPI_Isend(&count_ask, 1, MPI_INT, source, 0, MPI_COMM_WORLD, &request);
        MPI_Wait(&request, MPI_STATUS_IGNORE);
    }
    int count_recv = 0;
    for(int source = 0; source < world_size; source++){
        MPI_Status status;
        int count_receiving, flag_receiving = 0;
        if(world_rank == source){
            continue;
        }

        int tag = 1;
        while(!flag_receiving) {
            MPI_Iprobe(source, tag, MPI_COMM_WORLD, &flag_receiving, &status);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        MPI_Get_count(&status, MPI_INT, &count_receiving);
        auto* tasks_receiving = new int[count_receiving];
        flag_receiving = 0;
        MPI_Recv(tasks_receiving, count_receiving, MPI_INT, source, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if(count_receiving != 1 || tasks_receiving[0] > 0){
            pthread_mutex_lock(&mutex_tasks);
            tasks.insert(tasks.end(), tasks_receiving, tasks_receiving + count_receiving);
            sizeTasks += count_receiving;
            count_recv += count_receiving;
            pthread_mutex_unlock(&mutex_tasks);
            countReceivingTasks += count_receiving;
        }
        delete[] tasks_receiving;
    }
    flag_need_tasks = false;
    if(count_recv == 0 && numberTask >= sizeTasks){
        flag_iteration = false;
    }
}

void* getterThread(void*){
    while (flag_work){
        pthread_mutex_lock(&mutex_getter_global);
        while(!flag_iteration && flag_work){
            pthread_cond_wait(&cond_getter_global, &mutex_getter_global);
        }
        pthread_mutex_unlock(&mutex_getter_global);
        if(!flag_work){
            return nullptr;
        }
        if(flag_iteration) {
            getTasks();
        }
    }
    return nullptr;
}

int main(int argc, char* argv[]) {
    int provided;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);

    if(provided != MPI_THREAD_MULTIPLE){
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    pthread_t thread_getter;
    pthread_t thread_sender;
    double start = MPI_Wtime();
    if(pthread_create(&thread_getter, nullptr, getterThread, nullptr) != 0){
        MPI_Abort(MPI_COMM_WORLD, 2);
    }
    if(pthread_create(&thread_sender, nullptr, senderThread, nullptr) != 0){
        MPI_Abort(MPI_COMM_WORLD, 3);
    }
    while(num_iteration < count_iteration){
        pthread_mutex_lock(&mutex_tasks);
        generateTasks();
        pthread_mutex_unlock(&mutex_tasks);
        control();
        num_iteration++;
        MPI_Barrier(MPI_COMM_WORLD);
    }

    pthread_mutex_lock(&mutex_getter_local);
    pthread_mutex_lock(&mutex_getter_global);
    flag_iteration = false;
    flag_work = false;
    flag_need_tasks = false;
    pthread_cond_signal(&cond_getter_local);
    pthread_cond_signal(&cond_getter_global);
    pthread_mutex_unlock(&mutex_getter_local);
    pthread_mutex_unlock(&mutex_getter_global);

    if(pthread_join(thread_getter, nullptr) != 0){
        MPI_Abort(MPI_COMM_WORLD, 4);
    }
    if(pthread_join(thread_sender, nullptr) != 0){
        MPI_Abort(MPI_COMM_WORLD, 5);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double end = MPI_Wtime();
    auto* countsDoneTasks = new int[world_size];
    MPI_Gather(&countDoneTasks, 1, MPI_INT, countsDoneTasks, 1, MPI_INT, 0, MPI_COMM_WORLD);

    auto* countsSendingTasks = new int[world_size];
    MPI_Gather(&countSendingTasks, 1, MPI_INT, countsSendingTasks, 1, MPI_INT, 0, MPI_COMM_WORLD);

    auto* countsReceivingTasks = new int[world_size];
    MPI_Gather(&countReceivingTasks, 1, MPI_INT, countsReceivingTasks, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if(world_rank == 0) {
        for(int i = 0; i < world_size; i++){
            cout << "----------------------------------------------------------------------------------------------------" << endl <<
                    "Rank: " << i << "; countDone: " << countsDoneTasks[i] << "; count sending: " << countsSendingTasks[i] << "; countReceiving: " << countsReceivingTasks[i] << endl <<
                    "----------------------------------------------------------------------------------------------------" << endl;
        }
        cout << "Elapsed time: " << end - start << endl;
    }
    delete[] countsDoneTasks;
    MPI_Finalize();
    return 0;
}
