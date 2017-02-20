#ifndef BITS_HH
#define BITS_HH
#include <stdio.h>

#define _GET_OVERRIDE(_1, _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9,  _10,  _11,  _12,  _13,  _14,  _15,  _16, NAME, ...) NAME
#define BITS_1(_1) (_1::bits())
#define BITS_2(_1,_2) (_1::bits() | _2::bits())
#define BITS_3(_1,_2,_3) (_1::bits() | _2::bits() | _3::bits())
#define BITS_4(_1,_2,_3,_4) (_1::bits() | _2::bits() | _3::bits() | _4::bits())
#define BITS_5(_1,_2,_3,_4,_5) (_1::bits() | _2::bits() | _3::bits() | _4::bits() | _5::bits())
#define BITS_6(_1,_2,_3,_4,_5,_6) (_1::bits() | _2::bits() | _3::bits() | _4::bits() | _5::bits() | _6::bits())
#define BITS_7(_1,_2,_3,_4,_5,_6,_7) (_1::bits() | _2::bits() | _3::bits() | _4::bits() | _5::bits() | _6::bits() | _7::bits())
#define BITS_8(_1,_2,_3,_4,_5,_6,_7,_8) (_1::bits() | _2::bits() | _3::bits() | _4::bits() | _5::bits() | _6::bits() | _7::bits() | _8::bits())
#define BITS_9(_1,_2,_3,_4,_5,_6,_7,_8,_9) (_1::bits() | _2::bits() | _3::bits() | _4::bits() | _5::bits() | _6::bits() | _7::bits() | _8::bits() | _9::bits())
#define BITS_10(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10) (_1::bits() | _2::bits() | _3::bits() | _4::bits() | _5::bits() | _6::bits() | _7::bits() | _8::bits() | _9::bits() | _10::bits())
#define BITS_11(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11) (_1::bits() | _2::bits() | _3::bits() | _4::bits() | _5::bits() | _6::bits() | _7::bits() | _8::bits() | _9::bits() | _10::bits() | _11::bits())
#define BITS_12(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12) (_1::bits() | _2::bits() | _3::bits() | _4::bits() | _5::bits() | _6::bits() | _7::bits() | _8::bits() | _9::bits() | _10::bits() | _11::bits() | _12::bits())
#define BITS_13(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13) (_1::bits() | _2::bits() | _3::bits() | _4::bits() | _5::bits() | _6::bits() | _7::bits() | _8::bits() | _9::bits() | _10::bits() | _11::bits() | _12::bits() | _13::bits())
#define BITS_14(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14) (_1::bits() | _2::bits() | _3::bits() | _4::bits() | _5::bits() | _6::bits() | _7::bits() | _8::bits() | _9::bits() | _10::bits() | _11::bits() | _12::bits() | _13::bits() | _14::bits())
#define BITS_15(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15) (_1::bits() | _2::bits() | _3::bits() | _4::bits() | _5::bits() | _6::bits() | _7::bits() | _8::bits() | _9::bits() | _10::bits() | _11::bits() | _12::bits() | _13::bits() | _14::bits() | _15::bits())
#define BITS_16(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16) (_1::bits() | _2::bits() | _3::bits() | _4::bits() | _5::bits() | _6::bits() | _7::bits() | _8::bits() | _9::bits() | _10::bits() | _11::bits() | _12::bits() | _13::bits() | _14::bits() | _15::bits() | _16::bits())
#define BITS(...) _GET_OVERRIDE(__VA_ARGS__, BITS_16, BITS_15, BITS_14, BITS_13, BITS_12, BITS_11, BITS_10, BITS_9, BITS_8, BITS_7,  BITS_6, BITS_5, BITS_4, BITS_3, BITS_2, BITS_1)(__VA_ARGS__)

#define MASK_1(_1) (_1::mask())
#define MASK_2(_1,_2) (_1::mask() | _2::mask())
#define MASK_3(_1,_2,_3) (_1::mask() | _2::mask() | _3::mask())
#define MASK_4(_1,_2,_3,_4) (_1::mask() | _2::mask() | _3::mask() | _4::mask())
#define MASK_5(_1,_2,_3,_4,_5) (_1::mask() | _2::mask() | _3::mask() | _4::mask() | _5::mask())
#define MASK_6(_1,_2,_3,_4,_5,_6) (_1::mask() | _2::mask() | _3::mask() | _4::mask() | _5::mask() | _6::mask())
#define MASK_7(_1,_2,_3,_4,_5,_6,_7) (_1::mask() | _2::mask() | _3::mask() | _4::mask() | _5::mask() | _6::mask() | _7::mask())
#define MASK_8(_1,_2,_3,_4,_5,_6,_7,_8) (_1::mask() | _2::mask() | _3::mask() | _4::mask() | _5::mask() | _6::mask() | _7::mask() | _8::mask())
#define MASK_9(_1,_2,_3,_4,_5,_6,_7,_8,_9) (_1::mask() | _2::mask() | _3::mask() | _4::mask() | _5::mask() | _6::mask() | _7::mask() | _8::mask() | _9::mask())
#define MASK_10(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10) (_1::mask() | _2::mask() | _3::mask() | _4::mask() | _5::mask() | _6::mask() | _7::mask() | _8::mask() | _9::mask() | _10::mask())
#define MASK_11(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11) (_1::mask() | _2::mask() | _3::mask() | _4::mask() | _5::mask() | _6::mask() | _7::mask() | _8::mask() | _9::mask() | _10::mask() | _11::mask())
#define MASK_12(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12) (_1::mask() | _2::mask() | _3::mask() | _4::mask() | _5::mask() | _6::mask() | _7::mask() | _8::mask() | _9::mask() | _10::mask() | _11::mask() | _12::mask())
#define MASK_13(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13) (_1::mask() | _2::mask() | _3::mask() | _4::mask() | _5::mask() | _6::mask() | _7::mask() | _8::mask() | _9::mask() | _10::mask() | _11::mask() | _12::mask() | _13::mask())
#define MASK_14(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14) (_1::mask() | _2::mask() | _3::mask() | _4::mask() | _5::mask() | _6::mask() | _7::mask() | _8::mask() | _9::mask() | _10::mask() | _11::mask() | _12::mask() | _13::mask() | _14::mask())
#define MASK_15(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15) (_1::mask() | _2::mask() | _3::mask() | _4::mask() | _5::mask() | _6::mask() | _7::mask() | _8::mask() | _9::mask() | _10::mask() | _11::mask() | _12::mask() | _13::mask() | _14::mask() | _15::mask())
#define MASK_16(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16) (_1::mask() | _2::mask() | _3::mask() | _4::mask() | _5::mask() | _6::mask() | _7::mask() | _8::mask() | _9::mask() | _10::mask() | _11::mask() | _12::mask() | _13::mask() | _14::mask() | _15::mask() | _16::mask())
#define MASK(...) _GET_OVERRIDE(__VA_ARGS__, MASK_16, MASK_15, MASK_14, MASK_13, MASK_12, MASK_11, MASK_10, MASK_9, MASK_8, MASK_7,  MASK_6, MASK_5, MASK_4, MASK_3, MASK_2, MASK_1)(__VA_ARGS__)


class BitAttr {
public:
    uint32_t position;
    uint32_t length;
    constexpr BitAttr(const uint32_t position, const uint32_t length) : position(position), length(length){}
    constexpr BitAttr(BitAttr const &o) = default;        ///< Copy constructor
    constexpr BitAttr(BitAttr &&o) = default;             ///< Move constructor
    BitAttr& operator=(BitAttr const &o) = default;       ///< Copy assignment
    BitAttr& operator=(BitAttr &&o) = delete;            ///< Move assignment
};

//---------------

class Bit {
public:
    uint32_t value;
    BitAttr bitfield;
    //constexpr Bit1(const uint32_t position, const uint32_t length, const uint32_t value = 0) : bitfield(position, length), value(value){ }
    constexpr Bit(const BitAttr bitfield, const Bit value) : value(value.value), bitfield(value.bitfield){}
    constexpr Bit(const BitAttr bitfield, const uint32_t value = 0) : value(value), bitfield(bitfield){}
    template<typename T> constexpr Bit operator| (T x) const {return Bit(BitAttr(0, 32), bits() | Bit(BitAttr(0, 32), x).bits());}
    template<typename T> constexpr Bit operator& (T x) const {return Bit(BitAttr(0, 32), bits() & Bit(BitAttr(0, 32), x).bits());}
    template<typename T> constexpr Bit operator^ (T x) const {return Bit(BitAttr(0, 32), bits() ^ Bit(BitAttr(0, 32), x).bits());}
    constexpr Bit operator() (int x) const {return Bit(bitfield, x);} //functional
    constexpr Bit operator~ () const {return Bit(bitfield, ~value);}
    constexpr uint32_t pos() {return bitfield.position;}
    constexpr uint32_t len() {return bitfield.length;}
    constexpr uint32_t mask() {return ((1 << bitfield.length) - 1 ) << bitfield.position;}
    constexpr uint32_t val() {return value;}
    constexpr uint32_t bits() {return value << bitfield.position;}
    constexpr uint32_t bits(uint32_t v) {return (((1 << bitfield.length) - 1 ) << bitfield.position) & (v << bitfield.position);}
    constexpr explicit operator int() {return value;}
    constexpr explicit operator long unsigned int() {return value;}
};
constexpr int operator| (int y, Bit x) {return y | x.value;}
constexpr int operator& (int y, Bit x) {return y & x.value;}
constexpr int operator^ (int y, Bit x) {return y ^ x.value;}


#define BITDEF(name, bitfield, value) static constexpr auto name = Bit(bitfield, value)
template <typename T> class Reg;

class RegMap{
public:
    int offset;
    RegMap() : offset(0){
    }
    RegMap(int offset) : offset(offset){
    }
    RegMap(RegMap const &o) = default;        ///< Copy constructor
    RegMap(RegMap &&o) = default;             ///< Move constructor
    RegMap& operator=(RegMap const &o) = default;       ///< Copy assignment
    RegMap& operator=(RegMap &&o) = default;            ///< Move assignment
    void inline operator=(unsigned long int val) {
        volatile uint32_t* dev = reinterpret_cast<uint32_t*> (offset);
        *dev = val;
    }
    void inline operator=(Bit val) {
        volatile uint32_t* dev = reinterpret_cast<uint32_t*> (offset);
        *dev = val.bits();
    }
    void inline operator|=(Bit val) {
        volatile uint32_t* dev = reinterpret_cast<uint32_t*> (offset);
        *dev |= val.bits();
    }
    void inline operator&=(Bit val) {
        volatile uint32_t* dev = reinterpret_cast<uint32_t*> (offset);
        *dev &= val.bits();
    }
    void inline operator^=(Bit val) {
        volatile uint32_t* dev = reinterpret_cast<uint32_t*> (offset);
        *dev ^= val.bits();
    }
    int operator& (int y) {return int() & y;}
    int operator| (int y) {return int() | y;}
    int operator^ (int y) {return int() ^ y;}
    explicit operator int() {
        volatile uint32_t* dev = reinterpret_cast<uint32_t*> (offset);
        return *dev;
//        return reinterpret_cast<uint32_t*> (offset)[0];
    }
    template<typename U> explicit operator Reg<U>() {
        return Reg<U>(int());
    }
};

template <typename T, int N>  struct array{
    T value[N];
    T& operator[](int i) {
        return i < N ? value[i] : value[0];
    }
};

template <typename T> class Reg {
public:
    T x;
    Reg() {}
    template<typename U> Reg(U y) {
        x = int(y);
    }
    template<typename U> Reg(U &y) {
        x = int(y);
    }
    T operator=(Bit y) {return x = (T) y;}
    T operator=(T y) {return x = (T) y;}
    T operator=(void *y) {return x = reinterpret_cast<T>(y);}
    T operator=(volatile void *y) {return x = reinterpret_cast<T>(y);}
    template<typename U> T operator=(U y) {return x = reinterpret_cast<T>(y);}
    T operator=(RegMap y) {return x = int(y);}
    T operator=(RegMap &y) {return x = int(y);}
    constexpr explicit operator int() {return x;}
    constexpr explicit operator long int() {return x;}
    constexpr int operator==(Bit y) {return y.value == (x & y.mask());}
    template<typename U> T operator|(U y) {return x | y;}
    template<typename U> T operator&(U y) {return x & y;}
    template<typename U> T operator^(U y) {return x ^ y;}
};

#endif // BITS_HH
