#pragma once

#include <iostream>
#include <vector>

class BigNumber {
private:
    static constexpr int SEGMENT_SIZE {8};
    static constexpr int HEX_BASE {16};
    static constexpr int BITS_IN_UINT32 {32};

    std::vector<uint32_t> segments;

    enum class ShiftDirection {
        LEFT,
        RIGHT
    };

    template<typename Operation>
    BigNumber binaryOperation(const BigNumber& other, Operation op) const;
    BigNumber& shiftBits(unsigned int n, ShiftDirection direction);
    static void normalizeSize(std::vector<uint32_t>& a, std::vector<uint32_t>& b);

public:
    BigNumber() = default;
    BigNumber(const BigNumber& other) = default;
    explicit BigNumber(const std::string& hex);
    
    BigNumber& operator=(const BigNumber& other) = default;
    BigNumber operator~() const;
    BigNumber operator^(const BigNumber& other) const;
    BigNumber operator|(const BigNumber& other) const;
    BigNumber operator&(const BigNumber& other) const;
    BigNumber& operator>>=(unsigned int n);
    BigNumber& operator<<=(unsigned int n);
    bool operator==(const BigNumber& other) const;
    bool operator<(const BigNumber& other) const;
    BigNumber operator-(const BigNumber& other) const;
    BigNumber operator+(const BigNumber& other) const;

    void setHex(const std::string& hex);
    std::string getHex() const;
};