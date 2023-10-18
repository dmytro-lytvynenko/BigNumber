#include "big_number.h"

#include <sstream>
#include <iomanip>
#include <regex>

BigNumber::BigNumber(const std::string& hex) {
    setHex(hex);
}

void BigNumber::normalizeSize(std::vector<uint32_t>& a, std::vector<uint32_t>& b) {
    size_t maxSize = std::max(a.size(), b.size());
    a.resize(maxSize, 0);
    b.resize(maxSize, 0);
}

template<typename Operation>
BigNumber BigNumber::binaryOperation(const BigNumber& other, Operation op) const {
    BigNumber result;
    BigNumber other_copy(other);
    normalizeSize(result.segments, other_copy.segments);
    for (size_t i = 0; i < result.segments.size(); ++i) {
        result.segments[i] = op(this->segments[i], other_copy.segments[i]);
    }
    return result;
}

BigNumber& BigNumber::shiftBits(unsigned int n, ShiftDirection direction) {
    if (n == 0) return *this;

    const unsigned int segmentShifts = n / BITS_IN_UINT32;
    const unsigned int bitShifts = n % BITS_IN_UINT32;

    if (direction == ShiftDirection::RIGHT) {
        if (segmentShifts >= segments.size()) {
            segments.clear();
            return *this;
        }

        if (segmentShifts > 0) {
            segments.erase(segments.begin(), segments.begin() + segmentShifts);
        }

        for (size_t i = 0; i < segments.size(); ++i) {
            uint32_t shifted = bitShifts ? segments[i] >> bitShifts : 0;
            if (i < segments.size() - 1 && bitShifts) {
                shifted |= segments[i + 1] << (BITS_IN_UINT32 - bitShifts);
            }
            segments[i] = shifted;
        }
    } 
    else if (direction == ShiftDirection::LEFT) {
        if (bitShifts) {
            segments.push_back(0);
        }

        for (int i = segments.size() - 1; i > 0; --i) {
            segments[i] <<= bitShifts;
            segments[i] |= (segments[i - 1] >> (BITS_IN_UINT32 - bitShifts));
        }
        segments[0] <<= bitShifts;
    }

    return *this;
}

BigNumber BigNumber::operator~() const {
    BigNumber result(*this);
    for (auto& segment : result.segments) {
        segment = ~segment;
    }
    return result;
}

BigNumber& BigNumber::operator>>=(unsigned int n) {
    return shiftBits(n, ShiftDirection::RIGHT);
}

BigNumber& BigNumber::operator<<=(unsigned int n) {
    return shiftBits(n, ShiftDirection::LEFT);
}

BigNumber BigNumber::operator^(const BigNumber& other) const {
    return binaryOperation(other, [](uint32_t a, uint32_t b) { return a ^ b; });
}

BigNumber BigNumber::operator|(const BigNumber& other) const {
    return binaryOperation(other, [](uint32_t a, uint32_t b) { return a | b; });
}

BigNumber BigNumber::operator&(const BigNumber& other) const {
    return binaryOperation(other, [](uint32_t a, uint32_t b) { return a & b; });
}

bool BigNumber::operator==(const BigNumber& other) const {
    return segments == other.segments;
}

bool BigNumber::operator<(const BigNumber& other) const {
    if (segments.size() != other.segments.size()) {
        return segments.size() < other.segments.size();
    }
    for (int i = static_cast<int>(segments.size()) - 1; i >= 0; --i) {
        if (segments[i] != other.segments[i]) {
            return segments[i] < other.segments[i];
        }
    }
    return false;
}

BigNumber BigNumber::operator-(const BigNumber& other) const {
    BigNumber result;
    BigNumber a(*this);
    BigNumber b(other);

    normalizeSize(a.segments, b.segments);

    if (a < b) {
        throw std::runtime_error("Negative result not allowed in subtraction.");
    }

    int borrow = 0;
    for (size_t i = 0; i < a.segments.size(); ++i) {
        int64_t diff = int64_t(a.segments[i]) - b.segments[i] - borrow;
        if (diff < 0) {
            diff += (1LL << BITS_IN_UINT32);
            borrow = 1;
        } else {
            borrow = 0;
        }
        result.segments.push_back(uint32_t(diff));
    }

    return result;
}

BigNumber BigNumber::operator+(const BigNumber& other) const {
    BigNumber result;
    BigNumber a(*this);
    BigNumber b(other);

    normalizeSize(a.segments, b.segments);

    uint32_t carry = 0;
    for (size_t i = 0; i < a.segments.size(); ++i) {
        uint64_t sum = uint64_t(a.segments[i]) + b.segments[i] + carry;
        result.segments.push_back(uint32_t(sum));
        carry = sum >> BITS_IN_UINT32;
    }

    if (carry) {
        result.segments.push_back(carry);
    }

    return result;
}


void BigNumber::setHex(const std::string& hex) {
    std::regex hex_pattern("^([0-9a-fA-F]+)$");

    if (!std::regex_match(hex, hex_pattern)) {
        throw std::invalid_argument("Invalid hex number: " + hex);
    }

    segments.clear();
    int len = hex.length();

    for (int i = len; i > 0; i -= SEGMENT_SIZE) {
        int start = std::max(0, i - SEGMENT_SIZE);
        std::string segment = hex.substr(start, i - start);

        try {
            segments.push_back(std::stoul(segment, nullptr, HEX_BASE));
        } catch (const std::invalid_argument& e) {
            throw std::invalid_argument("Invalid argument encountered in stoul: " + segment);
        } catch (const std::out_of_range& e) {
            throw std::out_of_range("Value out of range encountered in stoul: " + segment);
        }
    }
}

std::string BigNumber::getHex() const {
    std::stringstream ss;
    bool firstSegment = true;

    for (auto it = segments.rbegin(); it != segments.rend(); ++it) {
        if (firstSegment) {
            ss << std::hex << *it;
            firstSegment = false;
        } else {
            ss << std::setfill('0') << std::setw(SEGMENT_SIZE) << std::hex << *it;
        }
    }
    return ss.str();
}
