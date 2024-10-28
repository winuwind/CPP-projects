#include "gtest/gtest.h"
#include "BitArray.h"

class BitArrayTest : public testing::Test {
protected:
    BitArrayTest() :
        data0_(),
        data1_(1005),
        data2_(data0_),
        data3_(data1_),
        counter_help(0)
    {
        for(size_t i = 0; i < 1005; i++){
            if(i % 6 == 0 || (i % 5 != 0 && i % 3 != 0)){
                data1_.reset(i);
            }
            else if(i % 5 == 0 || i % 3 == 0){
                data1_.set(i);
                counter_help++;
            }
        }
    }

    BitArray<size_t> data0_;
    BitArray<size_t> data1_;
    BitArray<size_t> data2_;
    BitArray<size_t> data3_;
    size_t counter_help;
};

TEST_F(BitArrayTest, Initially) {
    ASSERT_EQ(data0_.size(), 0);
    ASSERT_EQ(data1_.size(), 1005);
    ASSERT_EQ(data2_.size(), 0);
    ASSERT_EQ(data3_.size(), 1005);
    for(size_t i = 0; i < data0_.size(); i++){
        ASSERT_EQ(data0_[i], false);
    }
    for(size_t i = 0; i < data1_.size(); i++){
        if(i % 6 == 0){
            ASSERT_EQ(data1_[i], false);
        }
        else if(i % 5 == 0 || i % 3 == 0){
            ASSERT_EQ(data1_[i], true);
        }
        else{
            ASSERT_EQ(data1_[i], false);
        }
    }
    for(size_t i = 0; i < data2_.size(); i++){
        ASSERT_EQ(data2_[i], false);
    }
    for(size_t i = 0; i < data3_.size(); i++){
        ASSERT_EQ(data3_[i], false);
    }
}

TEST_F(BitArrayTest, MethodsSwap) {
    data3_ = data1_;
    data0_.swap(data1_);
    ASSERT_EQ(data1_.size(), 0);
    ASSERT_EQ(data0_.size(), 1005);
    for(size_t i = 0; i < data1_.size(); i++){
        ASSERT_EQ(data1_[i], data2_[i]);
    }
    for(size_t i = 0; i < data0_.size(); i++){
        ASSERT_EQ(data0_[i], data3_[i]);
    }
}

TEST_F(BitArrayTest, OperatorAssigment) {
    data2_ = data2_;
    ASSERT_EQ(data0_.size(), data2_.size());
    for(size_t i = 0; i < data2_.size(); i++){
        ASSERT_EQ(data2_[i], data0_[i]);
    }
    data2_ = data0_;
    ASSERT_EQ(data0_.size(), data2_.size());
    for(size_t i = 0; i < data2_.size(); i++){
        ASSERT_EQ(data2_[i], data0_[i]);
    }
}

TEST_F(BitArrayTest, MethodResize){
    size_t size = data3_.size();
    data2_ = data3_;
    data3_.resize(986);
    ASSERT_EQ(data3_.size(), 986);
    for(size_t i = 0; i < data3_.size(); i++){
        if(i < size){
            ASSERT_EQ(data3_[i], data2_[i]);
        }
        else{
            ASSERT_EQ(data3_[i], false);
        }
    }
    size = data3_.size();
    data2_ = data3_;
    data3_.resize(size / 2);
    ASSERT_EQ(data3_.size(), size / 2);
    for(size_t i = 0; i < data3_.size(); i++){
        if(i < size){
            ASSERT_EQ(data3_[i], data2_[i]);
        }
        else{
            ASSERT_EQ(data3_[i], false);
        }
    }
}

TEST_F(BitArrayTest, MethodClear){
    data3_.clear();
    ASSERT_EQ(data3_.size(), 0);
}

TEST_F(BitArrayTest, MethodPushBack){
    size_t size = data1_.size();
    for(size_t i = 0; i < 1000; i++){
        if(i % 6 == 0 || (i % 5 != 0 && i % 3 != 0)){
            data1_.push_back(false);
        }
        else if(i % 5 == 0 || i % 3 == 0){
            data1_.push_back(true);
            counter_help++;
        }
    }
    ASSERT_EQ(data1_.size(), size + 1000);
    for(size_t i = size; i < 1000 + size; i++){
        if((i - size) % 6 == 0){
            ASSERT_EQ(data1_[i], false);
        }
        else if((i - size) % 5 == 0 || (i - size) % 3 == 0){
            ASSERT_EQ(data1_[i], true);
        }
        else{
            ASSERT_EQ(data1_[i], false);
        }
    }
}

TEST_F(BitArrayTest, OperatorAndAssigment){
    data0_.resize(342);
    data3_ = data1_;
    data1_ &= data0_;
    ASSERT_EQ(data1_, data3_);
    data0_.resize(data1_.size());
    data1_ &= data0_;
    ASSERT_EQ(data0_.size(), data1_.size());
    ASSERT_EQ(data0_.size(), data3_.size());
    for(size_t i = 0; i < data2_.size(); i++){
        ASSERT_EQ(data1_[i], data0_[i] && data3_[i]);
    }
}

TEST_F(BitArrayTest, OperatorOrAssigment){
    data0_.resize(342);
    data3_ = data1_;
    data1_ |= data0_;
    ASSERT_EQ(data1_, data3_);
    data0_.resize(data1_.size());
    data1_ |= data0_;
    ASSERT_EQ(data0_.size(), data1_.size());
    ASSERT_EQ(data0_.size(), data3_.size());
    for(size_t i = 0; i < data1_.size(); i++){
        ASSERT_EQ(data1_[i], data0_[i] || data3_[i]);
    }
}

TEST_F(BitArrayTest, OperatorXorAssigment){
    data0_.resize(342);
    data3_ = data1_;
    data1_ ^= data0_;
    ASSERT_EQ(data1_, data3_);
    data0_.resize(data1_.size());
    data1_ ^= data0_;
    ASSERT_EQ(data0_.size(), data1_.size());
    ASSERT_EQ(data0_.size(), data3_.size());
    for(size_t i = 0; i < data2_.size(); i++){
        ASSERT_EQ(data1_[i], data0_[i] != data3_[i]);
    }
}

TEST_F(BitArrayTest, OperatorLeftShiftAssigment){
    data0_ <<= 78;
    ASSERT_EQ(data0_.size(), 0);
    data3_ = data1_;
    data0_ = data3_;
    data3_ <<= 11;
    ASSERT_EQ(data0_.size(), data3_.size());
    for(size_t i = 11; i < data0_.size(); i++){
        ASSERT_EQ(data3_[i - 11], data0_[i]);
    }
    if(data3_.size() >= 11) {
        for (size_t i = 0; i < 11; i++) {
            ASSERT_EQ(data3_[data3_.size() - 11 + i], false);
        }
    }
    data1_.resize(1233);
    data3_ = ~data1_;
    data0_ = data3_;
    data3_ <<= 11;
    ASSERT_EQ(data0_.size(), data3_.size());
    for(size_t i = 11; i < data0_.size(); i++){
        ASSERT_EQ(data3_[i - 11], data0_[i]);
    }
    if(data3_.size() >= 11) {
        for (size_t i = 0; i < 11; i++) {
            ASSERT_EQ(data3_[data3_.size() - 11 + i], false);
        }
    }
}

TEST_F(BitArrayTest, OperatorRightShiftAssigment){
    data0_ >>= 78;
    ASSERT_EQ(data0_.size(), 0);
    data3_ = data1_;
    data0_ = data3_;
    data3_ >>= 11;
    ASSERT_EQ(data0_.size(), data3_.size());
    for(size_t i = 11; i < data3_.size(); i++){
        ASSERT_EQ(data3_[i], data0_[i - 11]);
    }
    for(size_t i = 0; i < 11 && i < data3_.size(); i++){
        ASSERT_EQ(data3_[i], false);
    }
    data1_.resize(1233);
    data3_ = ~data1_;
    data0_ = data3_;
    data3_ >>= 11;
    ASSERT_EQ(data0_.size(), data3_.size());
    for(size_t i = 11; i < data3_.size(); i++){
        if(data3_[i] != data0_[i - 11]){
        }
        ASSERT_EQ(data3_[i], data0_[i - 11]);
    }
    for(size_t i = 0; i < 11 && i < data3_.size(); i++){
        ASSERT_EQ(data3_[i], false);
    }
}

TEST_F(BitArrayTest, OperatorLeftShift){
    data3_ = data1_;
    data0_ = data3_;
    data3_ = data3_ << 11;
    ASSERT_EQ(data0_.size(), data3_.size());
    for(size_t i = 11; i < data0_.size(); i++){
        ASSERT_EQ(data3_[i - 11], data0_[i]);
    }
    if(data3_.size() >= 11) {
        for (size_t i = 0; i < 11; i++) {
            ASSERT_EQ(data3_[data3_.size() - 11 + i], false);
        }
    }
    data1_.resize(1233);
    data3_ = ~data1_;
    data0_ = data3_;
    data3_ = data3_ << 11;
    ASSERT_EQ(data0_.size(), data3_.size());
    for(size_t i = 11; i < data0_.size(); i++){
        ASSERT_EQ(data3_[i - 11], data0_[i]);
    }
    if(data3_.size() >= 11) {
        for (size_t i = 0; i < 11; i++) {
            ASSERT_EQ(data3_[data3_.size() - 11 + i], false);
        }
    }
}

TEST_F(BitArrayTest, OperatorRightShift){
    data3_ = data1_;
    data0_ = data3_;
    data3_ = data3_ >> 11;
    ASSERT_EQ(data0_.size(), data3_.size());
    for(size_t i = 11; i < data3_.size(); i++){
        ASSERT_EQ(data3_[i], data0_[i - 11]);
    }
    for(size_t i = 0; i < 11 && i < data3_.size(); i++){
        ASSERT_EQ(data3_[i], false);
    }
    data1_.resize(1233);
    data3_ = ~data1_;
    data0_ = data3_;
    data3_ = data3_ >> 11;
    ASSERT_EQ(data0_.size(), data3_.size());
    for(size_t i = 11; i < data3_.size(); i++){
        ASSERT_EQ(data3_[i], data0_[i - 11]);
    }
    for(size_t i = 0; i < 11 && i < data3_.size(); i++){
        ASSERT_EQ(data3_[i], false);
    }
}

TEST_F(BitArrayTest, MethodSet){
    data1_.resize(5678);
    data3_ = data1_;
    size_t size = data1_.size();
    data1_.set(10000);
    ASSERT_EQ(data1_.size(), size);
    ASSERT_EQ((data1_ == data3_), true);
    data1_.set(7);
    ASSERT_EQ(data1_.size(), size);
    ASSERT_EQ(data1_[7], true);
    data1_.set(7, false);
    ASSERT_EQ(data1_.size(), size);
    ASSERT_EQ(data1_[7], false);
    data1_.set();
    ASSERT_EQ(data1_.size(), size);
    for(size_t i = 0; i < size; i++){
        ASSERT_EQ(data1_[i], true);
    }
}

TEST_F(BitArrayTest, MethodReset){
    data1_.resize(56789);
    size_t size = data1_.size();
    data3_ = data1_;
    data1_.reset(100000);
    ASSERT_EQ(data1_.size(), size);
    ASSERT_EQ((data1_ == data3_), true);
    data1_.reset(5);
    ASSERT_EQ(data1_.size(), size);
    ASSERT_EQ(data1_[5], false);
    data1_.reset();
    ASSERT_EQ(data1_.size(), size);
    for(size_t i = 0; i < size; i++){
        ASSERT_EQ(data1_[i], false);
    }
}

TEST_F(BitArrayTest, MethodAny){
    data1_.resize(12345);
    data1_.reset();
    size_t size = data1_.size();
    bool result = data1_.any();
    ASSERT_EQ(data1_.size(), size);
    ASSERT_EQ(result, false);
    data1_.set(7);
    result = data1_.any();
    ASSERT_EQ(data1_.size(), size);
    ASSERT_EQ(result, true);
    data1_.reset();
    data1_.set(12344);
    result = data1_.any();
    ASSERT_EQ(data1_.size(), size);
    ASSERT_EQ(result, true);
}

TEST_F(BitArrayTest, MethodNone){
    data1_.resize(87654);
    data1_.push_back(true);
    size_t size = data1_.size();
    bool result = data1_.none();
    ASSERT_EQ(data1_.size(), size);
    ASSERT_EQ(result, false);
    data1_.reset();
    result = data1_.none();
    ASSERT_EQ(data1_.size(), size);
    ASSERT_EQ(result, true);
}

TEST_F(BitArrayTest, OperatorNo){
    data0_ = data3_;
    ASSERT_EQ((~data3_).size(), data3_.size());
    ASSERT_EQ(data0_, data3_);
    for(size_t i = 0; i < data3_.size(); i++){
        ASSERT_EQ((~data3_)[i], !data3_[i]);
    }
}

TEST_F(BitArrayTest, MethodCount){
    data0_ = data1_;
    size_t count = data1_.count();
    ASSERT_EQ(data0_, data1_);
    ASSERT_EQ(count, /*value for data3_ which we count when init*/ counter_help);
}

TEST_F(BitArrayTest, OperatorGetBit){
    ASSERT_EQ(data0_[100], false);
}

//TEST_F(BitArrayTest, MethodSize){
//
//}

TEST_F(BitArrayTest, MethodEmpty){
    data0_.clear();
    ASSERT_EQ(data0_.empty(), true);
    ASSERT_EQ(data1_.empty(), false);
}

TEST_F(BitArrayTest, MethodToString){
    data0_ = data1_;
    std::string data_in_string = data1_.to_string();
    ASSERT_EQ(data0_, data1_);
    for(size_t i = 0; i < data1_.size(); i += 8){
        char byte = 0;
        int bit = 1 << 7;
        for(size_t j = 0; j < 8 && i + j < data1_.size(); j++){
            if(data1_[i + j]) {
                byte |= bit;
            }
            bit >>= 1;
        }
        ASSERT_EQ(data_in_string[i / 8], byte);
    }
}

TEST_F(BitArrayTest, OperatorEqual){
    data2_.resize(1);
    for(size_t i = 0; i < 34567; i++){
        data0_.push_back((bool) (i % 2));
    }
    ASSERT_EQ(data0_ == data0_, true);
    ASSERT_EQ(data0_ == (data0_ >> 11), false);
    ASSERT_EQ(data0_ == data2_, false);
}

TEST_F(BitArrayTest, OperatorNotEqual){
    data2_.resize(1);
    for(size_t i = 0; i < 34567; i++){
        data0_.push_back((bool) (i % 2));
    }
    ASSERT_EQ(data0_ != data0_, false);
    ASSERT_EQ(data0_ != (data0_ >> 10), true);
    ASSERT_EQ(data0_ != data2_, true);
}

TEST_F(BitArrayTest, OperatorAnd){
    data0_.resize(342);
    data3_ = data2_;
    data2_ = data2_ & data0_;
    ASSERT_EQ(data2_, data3_);
    data0_.resize(data2_.size());
    data2_ = data2_ & data0_;
    ASSERT_EQ(data0_.size(), data2_.size());
    ASSERT_EQ(data0_.size(), data3_.size());
    for(size_t i = 0; i < data2_.size(); i++){
        ASSERT_EQ(data2_[i], data0_[i] && data3_[i]);
    }
}

TEST_F(BitArrayTest, OperatorOr){
    data0_.resize(342);
    data3_ = data2_;
    data2_ = data2_ | data0_;
    ASSERT_EQ(data2_, data3_);
    data0_.resize(data2_.size());
    data2_ = data2_ | data0_;
    ASSERT_EQ(data0_.size(), data2_.size());
    ASSERT_EQ(data0_.size(), data3_.size());
    for(size_t i = 0; i < data2_.size(); i++){
        ASSERT_EQ(data2_[i], data0_[i] || data3_[i]);
    }
}

TEST_F(BitArrayTest, OperatorXor){
    data0_.resize(342);
    data3_ = data2_;
    data2_ = data2_ ^ data0_;
    ASSERT_EQ(data2_, data3_);
    data0_.resize(data2_.size());
    data2_ = data2_ ^ data0_;
    ASSERT_EQ(data0_.size(), data2_.size());
    ASSERT_EQ(data0_.size(), data3_.size());
    for(size_t i = 0; i < data2_.size(); i++){
        ASSERT_EQ(data2_[i], data0_[i] != data3_[i]);
    }
}
