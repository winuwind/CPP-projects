#ifndef BITARRAY_BITARRAY_H
#define BITARRAY_BITARRAY_H

#include <iostream>

size_t null = 0;

template<typename Align, size_t AlignSize = sizeof(Align) * 8>
class BitArray
{
private:
    std::size_t sizeInBits;
    std::size_t alignedSize;
    Align *data;
public:
    /***************************************************************************
*   Method     : BitArray - constructor
*   Description: This is the BitArray constructor.
*   Parameters : None
*   Returned   : None
***************************************************************************/
    BitArray():
            sizeInBits(null),
            alignedSize(null),
            data(nullptr){}

    /***************************************************************************
*   Method     : ~BitArray - destructor
*   Description: This is the BitArray destructor.  At this point it's
*                just a place holder.
*   Parameters : None
*   Effects    : None
*   Returned   : None
***************************************************************************/
    ~BitArray() {
        if(this->alignedSize) {
            delete[] this->data;
            this->data = nullptr;
            this->sizeInBits = null;
            this->alignedSize = null;
        }
    }

    /***************************************************************************
*   Method     : BitArray - constructor
*   Description: This is the BitArray constructor.  It reserves memory
*                for the vector storing the array.
*   Parameters : num_bits - number of bits in the array
*   Effects    : Allocates vector for array bits
*   Returned   : None
***************************************************************************/
    explicit BitArray(size_t num_bits):
            sizeInBits(num_bits),
            alignedSize((num_bits + AlignSize - 1) / AlignSize),
            data(new Align[alignedSize]{})
    {
        for(size_t i = 0; i < this->alignedSize; i++){
            this->data[i] = (Align) null;
        }
    }

    /***************************************************************************
*   Method     : BitArray - copy constructor
*   Description: This is the BitArray copy constructor.  It copy other BitArray.
*   Parameters : BitArray & b - BitArray references
*   Effects    : Allocates vector for array bits
*   Returned   : None
***************************************************************************/
    BitArray(const BitArray& b):
            sizeInBits(b.sizeInBits),
            alignedSize(b.alignedSize),
            data(new Align[alignedSize]{}) {
        for (size_t i = 0; i < alignedSize; ++i) {
            data[i] = b.data[i];
        }
    }


    /***************************************************************************
*   Method     : swap() - function
*   Description: exchanging values between source bit array and this array
*   Parameters : b - Source bit array reference
*   Effects    : Actual variable renaming
*   Returned   : Reference to this array after operations
***************************************************************************/
    void swap(BitArray& b){
        if(this != &b){
            BitArray helper(b);
            BitArray & helper_ref(helper);
            BitArray & this_ref(*this);
            b.operator=(this_ref);
            this_ref.operator=(helper_ref);
        }
    }

    /***************************************************************************
*   Method     : operator=
*   Description: overload of the = operator.  Copies source contents into
*                this bit array.
*   Parameters : b - Source bit array reference
*   Effects    : Source bit array contents are copied into this array
*   Returned   : Reference to this array after copy
***************************************************************************/
    BitArray& operator=(const BitArray& b){
        if(this == &b){
            return *this;
        }
        if(this->sizeInBits == null || this->data == nullptr) {
            this->data = new Align[b.alignedSize];
        }
        else if(this->alignedSize != b.alignedSize) {
            delete[] this->data;
            this->data = new Align[b.alignedSize];
        }
        this->sizeInBits = b.sizeInBits;
        this->alignedSize = b.alignedSize;
        for(size_t i = 0; i < this->alignedSize; i++){
            this->data[i] = b.data[i];
        }
        return *this;
    }

    /***************************************************************************
*   Method     : resize() - function
*   Description: Changes the size of the array. In case of expansion, new
*                elements initialized with value.
*   Parameters : num_bits - new size
*   Effects    : New size of bitset
*   Returned   : None
***************************************************************************/
    void resize(size_t num_bits) {
        BitArray other(num_bits);
        BitArray & other_ref(other);
        for(size_t i = 0; i < other_ref.alignedSize; i++){
            if(i >= this->alignedSize){
                other.data[i] = (Align) null;
            }
            else {
                other.data[i] = this->data[i];
            }
        }
        if(num_bits > this->sizeInBits){
            size_t help = AlignSize - (this->sizeInBits % AlignSize);
            size_t pat = null;
            for(size_t i = help, j = ((size_t) 1) << help; i < AlignSize; i++, j = j << 1){
                pat |= j;
            }
            other.data[this->sizeInBits / AlignSize] &= (Align) pat;
        }
        this->operator=(other);
    }

    /***************************************************************************
*   Method     : clear() - function
*   Description: This method delete data.
*   Parameters : None
*   Effects    : delete data and set data = nullptr, set size of bit array = 0.
*   Returned   : None
***************************************************************************/
    void clear() {
        if(this->alignedSize){
            this->alignedSize = null;
            this->sizeInBits = null;
            delete[] this->data;
            this->data = nullptr;
        }
    }

    /***************************************************************************
*   Method     : push_back() - function
*   Description: adds a new element to the end of the bitset
*   Parameters : bit - new element value
*   Effects    : None
*   Returned   : None
***************************************************************************/
    void push_back(bool bit) {
        if(this->alignedSize == null){
            this->alignedSize++;
            this->sizeInBits++;
            this->data = new Align[alignedSize];
            this->data[0] = (Align) null;
            if(bit) {
                size_t pat = 1;
                pat <<= (AlignSize - 1);
                this->data[0] |= (Align) pat;
            }
        }
        else if((this->sizeInBits + AlignSize) / AlignSize != this->alignedSize) {
            BitArray arr_copy(*this);
            delete[] this->data;
            this->sizeInBits++;
            this->alignedSize++;
            this->data = new Align[alignedSize];
            for (size_t i = 0; i < arr_copy.alignedSize; i++) {
                this->data[i] = (Align) null;
                this->data[i] = arr_copy.data[i];
            }
            this->data[this->alignedSize - 1] = (Align) null;
            if(bit) {
                size_t pat = 1;
                pat <<= (AlignSize - 1);
                this->data[this->alignedSize - 1] |= (Align) pat;
            }
        }
        else if(bit){
            size_t help = AlignSize - (this->sizeInBits % AlignSize);
            auto x = (Align) ((null + 1) << (help - 1));
            this->data[this->alignedSize - 1] |= x;
            this->sizeInBits++;
        }
        else{
            this->sizeInBits++;
        }
    }

    /***************************************************************************
*   Method     : operator&=
*   Description: overload of the &= operator.  Performs a bitwise and
*                between the source array and this bit array.  This bit
*                array will contain the result.
*   Parameters : b - Source bit array reference
*   Effects    : Results of bitwise and are stored in this array
*   Returned   : Reference to this array after and
***************************************************************************/
    BitArray& operator&=(const BitArray& b) {
        if(this->sizeInBits != b.sizeInBits){
            std::cout << "operator&=: different size of arrays\n";
            return *this;
        }
        for(size_t i = 0; i < this->alignedSize; i++){
            this->data[i] = this->data[i] & b.data[i];
        }
        return *this;
    }

    /***************************************************************************
*   Method     : operator|=
*   Description: overload of the |= operator.  Performs a bitwise xor
*                between the source array and this bit array.  This bit
*                array will contain the result.
*   Parameters : b - Source bit array reference
*   Effects    : Results of bitwise xor are stored in this array
*   Returned   : Reference to this array after or
***************************************************************************/
    BitArray& operator|=(const BitArray& b){
        if(this->sizeInBits != b.sizeInBits){
            std::cout << "operator|=: different size of arrays\n";
            return *this;
        }
        for(size_t i = 0; i < this->alignedSize; i++){
            this->data[i] = this->data[i] | b.data[i];
        }
        return *this;
    }

    /***************************************************************************
*   Method     : operator^=
*   Description: overload of the ^= operator.  Performs a bitwise xor
*                between the source array and this bit array.  This bit
*                array will contain the result.
*   Parameters : b - Source bit array reference
*   Effects    : Results of bitwise xor are stored in this array
*   Returned   : Reference to this array after xor
***************************************************************************/
    BitArray& operator^=(const BitArray& b) {
        if(this->sizeInBits != b.sizeInBits){
            std::cout << "operator^=: different size of arrays\n";
            return *this;
        }
        for(size_t i = 0; i < this->alignedSize; i++){
            this->data[i] = this->data[i] ^ b.data[i];
        }
        return *this;
    }

    /***************************************************************************
*   Method     : operator<<=
*   Description: overload of the <<= operator.  Performs a left shift on
*                this bit array.  This bit array will contain the result.
*   Parameters : n - number of bit positions to shift
*   Effects    : Results of the shifts are stored in this array
*   Returned   : Reference to this array after shift
***************************************************************************/
    BitArray& operator<<=(size_t n){
        if(this->alignedSize == 0){
            return *this;
        }
        BitArray other(*this);
        delete[] this->data;
        this->sizeInBits = null;
        this->alignedSize = null;
        this->data = nullptr;
        for(size_t i = n; i < other.sizeInBits + n; i++){
            if(i < other.sizeInBits) {
                this->push_back(other.operator[](i));
            }
            else{
                this->push_back(false);
            }
        }
        return *this;
    }

    /***************************************************************************
*   Method     : operator>>=
*   Description: overload of the >>= operator.  Performs a right shift on
*                this bit array.  This bit array will contain the result.
*   Parameters : n - number of bit positions to shift
*   Effects    : Results of the shifts are stored in this array
*   Returned   : Reference to this array after shift
***************************************************************************/
    BitArray& operator>>=(size_t n){
        if(this->alignedSize == null){
            return *this;
        }
        BitArray other(*this);
        delete[] this->data;
        this->sizeInBits = null;
        this->alignedSize = null;
        this->data = nullptr;
        for(size_t i = 0; i < other.sizeInBits; i++){
            if(i < n) {
                this->push_back(false);
            }
            else{
                this->push_back(other.operator[](i - n));
            }
        }
        return *this;
    }

    /***************************************************************************
*   Method     : operator<<
*   Description: overload of the << operator.  Performs a bitwise left
*                shift of this bit array.
*   Parameters : n - the number of bits to shift left
*   Effects    : None
*   Returned   : result of bitwise left shift
***************************************************************************/
    BitArray operator<<(size_t n) const {
        BitArray other;
        for(size_t i = n; i < this->sizeInBits + n; i++){
            if(i < this->sizeInBits) {
                other.push_back(this->operator[](i));
            }
            else{
                other.push_back(false);
            }
        }
        return other;
    }

    /***************************************************************************
*   Method     : operator>>
*   Description: overload of the >> operator.  Performs a bitwise right
*                shift of this bit array.
*   Parameters : n - the number of bits to shift right
*   Effects    : None
*   Returned   : result of bitwise right shift
***************************************************************************/
    BitArray operator>>(size_t n) const {
        BitArray other;
        for(size_t i = 0; i < this->sizeInBits; i++){
            if(i < n) {
                other.push_back(false);
            }
            else{
                other.push_back(this->operator[](i - n));
            }
        }
        return other;
    }

    /***************************************************************************
*   Method     : set
*   Description: This method sets one bit in the bit array to 1.
*   Parameters : n - the bit number to be set true.
*   Effects    : Bit with number n is set to 1.
*   Returned   : BitArray &
***************************************************************************/
    BitArray& set(size_t n){
        if(n > this->sizeInBits){
            return *this;
        }
        this->data[n / AlignSize] |= (Align) ((null + 1) << (AlignSize - n % AlignSize - 1));
        return *this;
    }

    /***************************************************************************
*   Method     : set
*   Description: This method sets every bit in the bit array to 1.
*   Parameters : None
*   Effects    : Each of the bits used in the bit array are set to 1.
*   Returned   : BitArray &
***************************************************************************/
    BitArray& set(){
        size_t pat = 1;
        for(size_t i = 0, j = 1; i < AlignSize; i++, j = j << 1){
            pat |= j;
        }
        for(size_t i = 0; i < this->alignedSize; i++){
            this->data[i] = (Align) pat;
        }
        return *this;
    }

    /***************************************************************************
*   Method     : reset
*   Description: This method sets one bit in the bit array to 0.
*   Parameters : n - number
*   Effects    : Bit with number n is set to 0.
*   Returned   : BitArray &
***************************************************************************/
    BitArray& reset(size_t n){
        if(n > this->sizeInBits){
            return *this;
        }
        size_t help = AlignSize - (n % AlignSize);
        size_t pat = null;
        for(size_t i = 0, j = 1; i < AlignSize; i++, j <<= 1){
            if(i == help - 1){
                continue;
            }
            pat |= j;
        }
        this->data[n / AlignSize] &= (Align) pat;
        return *this;
    }

    /***************************************************************************
*   Method     : reset
*   Description: This method sets every bit in the bit array to 1.
*   Parameters : None
*   Effects    : Each of the bits used in the bit array are set to 0.
*   Returned   : BitArray &
***************************************************************************/
    BitArray& reset(){
        for(size_t i = 0; i < this->alignedSize; i++){
            this->data[i] = (Align) null;
        }
        return *this;
    }

    /***************************************************************************
*   Method     : any() - function
*   Description: checks if all bits in a bitset are true
*   Parameters : None
*   Effects    : None
*   Returned   : false is all bits are false
***************************************************************************/
    [[nodiscard]] bool any() const{
        for(size_t i = 0; i < this->alignedSize - 1; i++){
            if(this->data[i] > (Align) null){
                return true;
            }
        }
        size_t help = AlignSize - (this->sizeInBits % AlignSize);
        size_t pat = null;
        for(size_t i = help, j = 1 << help; i < AlignSize; i++, j = j << 1){
            pat |= j;
        }
        if((this->data[this->alignedSize - 1] & (Align) pat) > (Align) null){
            return true;
        }
        return false;
    }

    /***************************************************************************
*   Method     : none() - function
*   Description: checks if all bits in a bitset are true
*   Parameters : None
*   Effects    : None
*   Returned   : true is all bits are false
***************************************************************************/
    [[nodiscard]] bool none() const{
        return !this->any();
    }

    /***************************************************************************
*   Method     : operator~
*   Description: overload of the ~ operator.  Negates all non-spare bits in
*                bit array
*   Parameters : None
*   Effects    : None
*   Returned   : value of this after bitwise not
***************************************************************************/
    BitArray operator~() const{
        BitArray other(*this);
        for(size_t i = 0; i < this->alignedSize; i++){
//            other.data[i] = ~this->data[i];
            size_t pat = -1;
            other.data[i] = this->data[i] ^ pat;
        }
        return other;
    }

    /***************************************************************************
*   Method     : count() - function
*   Description: The function counts the number of true bits
*   Parameters : None
*   Effects    : None
*   Returned   : count true bits
***************************************************************************/
    [[nodiscard]] size_t count() const{
        size_t counter = null;
        for(size_t i = 0; i < this->alignedSize - 1; i++){
            for(size_t index = 0, j = 1; index < AlignSize; index++, j = j << 1){
                if((this->data[i] & (Align) j) > (Align) null){
                    counter++;
                }
            }
        }
        size_t help = AlignSize - this->sizeInBits % AlignSize;
        for(size_t index = help, j = 1 << help; index < AlignSize; index++, j = j << 1){
            if((this->data[this->alignedSize - 1] & (Align) j) > (Align) null){
                counter++;
            }
        }
        return counter;
    }

    /***************************************************************************
*   Method     : operator[]
*   Description: Overload of the [] operator.  This method returns the
*                value of a bit in the bit array.
*   Parameters : i - index of array bit
*   Effects    : None
*   Returned   : The value of the specified bit.
***************************************************************************/
    bool operator[](size_t i) const{
        auto pat = (Align) ((null + 1) << (AlignSize - i % AlignSize - 1));
        auto value = (bool) (this->data[i / AlignSize] & pat);
        return value;
    }

    /***************************************************************************
*   Method     : size() - function
*   Description: The function which return size of bit array
*   Parameters : None
*   Effects    : None
*   Returned   : size_t - size of bit array
***************************************************************************/
    [[nodiscard]] size_t size() const{
        return this->sizeInBits;
    }

    /***************************************************************************
*   Method     : empty() - function
*   Description: returned bool value: if size of bit array isn't null - false, else - true
*   Parameters : None
*   Effects    : None
*   Returned   : bool - true if size of bit array is null
***************************************************************************/
    [[nodiscard]] bool empty() const {
        if(this->sizeInBits == null){
            return true;
        }
        return false;
    }

    /***************************************************************************
*   Method     : to string() - function
*   Description: Returns a string representation of an array
*   Parameters : None
*   Effects    : None
*   Returned   : A string representation of an array
***************************************************************************/
    [[nodiscard]] std::string to_string() const {
        std::string bytes;
        char symbol = '\0';
        for(size_t i = 0; i < this->sizeInBits; i++){
            if(this->operator[](i)){
                symbol = (char) (symbol | ((char) 1 << (7 - i % 8)));
            }
            if(i % 8 == 7){
                bytes.append(&symbol);
                symbol = '\0';
            }
        }
        if(symbol != '\0'){
            bytes.append(&symbol);
            symbol = '\0';
        }
        return bytes;
    }
};


/***************************************************************************
*   Method     : operator==
*   Description: overload of the == operator
*   Parameters : a, b - bit array references to compare
*   Effects    : None
*   Returned   : True if this == other.  Otherwise false.
***************************************************************************/
template<typename Align, size_t AlignSize = sizeof(Align) * 8>
bool operator==(const BitArray<Align> & a, const BitArray<Align> & b) {
    if(a.size() != b.size()){
        return false;
    }
    else{
        for(size_t i = 0; i < a.size(); i++){
            if(a[i] != b[i]){
                return false;
            }
        }
        return true;
    }
}

/***************************************************************************
*   Method     : operator!=
*   Description: overload of the != operator
*   Parameters : a, b - bit array references to compare
*   Effects    : None
*   Returned   : True if this != other.  Otherwise false.
***************************************************************************/
template<typename Align, size_t AlignSize = sizeof(Align) * 8>
bool operator!=(const BitArray<Align> & a, const BitArray<Align> & b) {
    if(a.size() != b.size()){
        return true;
    }
    else{
        for(size_t i = 0; i < a.size(); i++){
            if(a[i] != b[i]){
                return true;
            }
        }
        return false;
    }
}

/***************************************************************************
*   Method     : operator&
*   Description: overload of the & operator.  Performs a bitwise and
*                between the source arrays.
*   Parameters : b1, b2 - bit array references on righthand side of &
*   Effects    : None
*   Returned   : BitArray - result operation &
***************************************************************************/
template<typename Align, size_t AlignSize = sizeof(Align) * 8>
BitArray<Align> operator&(const BitArray<Align>& b1, const BitArray<Align>& b2) {
    if(b1.size() != b2.size()){
        std::cout << "operator&: different lengths\n";
        return b1;
    }
    else{
        BitArray<Align> other;
        for(size_t i = 0; i < b1.size(); i++){
            other.push_back((bool) (b1[i] && b2[i]));
        }
        return other;
    }
}

/***************************************************************************
*   Method     : operator|
*   Description: overload of the | operator.  Performs a bitwise and
*                between the source arrays.
*   Parameters : b1, b2 - bit array references on righthand side of |
*   Effects    : None
*   Returned   : BitArray - result operation |
***************************************************************************/
template<typename Align, size_t AlignSize = sizeof(Align) * 8>
BitArray<Align> operator|(const BitArray<Align> & b1, const BitArray<Align> & b2) {
    if(b1.size() != b2.size()){
        std::cout << "operator|: different lengths\n";
        return b1;
    }
    else{
        BitArray<Align> other;
        for(size_t i = 0; i < b1.size(); i++){
            other.push_back((bool) (b1[i] || b2[i]));
        }
        return other;
    }
}

/***************************************************************************
*   Method     : operator^
*   Description: overload of the ^ operator.  Performs a bitwise and
*                between the source arrays.
*   Parameters : b1, b2 - bit array references on righthand side of ^
*   Effects    : None
*   Returned   : BitArray - result operation ^
***************************************************************************/
template<typename Align, size_t AlignSize = sizeof(Align) * 8>
BitArray<Align> operator^(const BitArray<Align> & b1, const BitArray<Align> & b2) {
    if(b1.size() != b2.size()){
        std::cout << "operator^: different lengths\n";
        return b1;
    }
    else{
        BitArray<Align> other;
        for(size_t i = 0; i < b1.size(); i++){
            other.push_back((bool) (b1[i] != b2[i]));
        }
        return other;
    }
}

#endif //BITARRAY_BITARRAY_H