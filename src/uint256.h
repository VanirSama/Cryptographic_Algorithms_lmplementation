#pragma once
#include<iostream>
#include <array>
#include <algorithm>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <iomanip>
using namespace std;

class uint256_t {
public:
    
    uint256_t() {
        values.fill(0);
    }

    uint256_t(uint64_t value) {
        values.fill(0);
        values[0] = static_cast<uint32_t>(value);
        values[1] = static_cast<uint32_t>(value >> 32);
    }

    uint256_t(const uint256_t& other) = default;

    ~uint256_t() {};

    array<uint32_t, 8> ExportValues() {
        return values;
    }

    uint256_t& operator=(const uint256_t& other) = default;

    uint256_t operator+(const uint256_t& other) const { // Оператор сложения
        uint256_t result;
        uint64_t carry = 0;
        for (size_t i = 0; i < 8; ++i) {
            uint64_t sum = static_cast<uint64_t>(values[i]) + other.values[i] + carry;
            result.values[i] = static_cast<uint32_t>(sum);
            carry = (sum >> 32);
        }
        return result;
    }

    uint256_t& operator++() { // Префиксный оператор ++
        *this = *this + uint256_t(1);
        return *this;
    }

    uint256_t operator++(int) { // Постфиксный оператор ++
        uint256_t temp = *this;
        ++(*this);
        return temp;
    }

    uint256_t operator-(const uint256_t& other) const {
        uint256_t result;
        int64_t borrow = 0;
        for (size_t i = 0; i < 8; ++i) {
            int64_t sub = static_cast<int64_t>(values[i]) - other.values[i] - borrow;
            if (sub < 0) {
                borrow = 1;
                sub += 0x100000000;
            }
            else {
                borrow = 0;
            }
            result.values[i] = static_cast<uint32_t>(sub);
        }
        return result;
    }

    uint256_t& operator--() { // Префиксный оператор --
        *this = *this - uint256_t(1);
        return *this;
    }

    uint256_t operator--(int) { // Постфиксный оператор --
        uint256_t temp = *this;
        --(*this);
        return temp;
    }

    uint256_t operator*(const uint256_t& other) const { // Оператор умножения
        uint256_t result;
        for (size_t i = 0; i < 8; ++i) {
            uint64_t carry = 0;
            if (values[i] == 0) continue;
            for (size_t j = 0; j < 8; ++j) {
                if (i + j >= 8) break;
                uint64_t prod = static_cast<uint64_t>(values[i]) * other.values[j] + result.values[i + j] + carry;
                result.values[i + j] = static_cast<uint32_t>(prod);
                carry = prod >> 32;
            }
        }
        return result;
    }

    uint256_t operator/(const uint256_t& other) const { // Оператор деления
        if (other.is_zero()) {
            throw std::overflow_error("Division by zero");
        }
        uint256_t dividend = *this;
        uint256_t divisor = other;
        uint256_t result;
        uint256_t current;
        for (int i = 255; i >= 0; --i) {
            current = current <<= 1;
            if (dividend.get_bit(i)) {
                current.set_bit(0);
            }
            if (current >= divisor) {
                current = current - divisor;
                result.set_bit(i);
            }
        }
        return result;
    }

    uint256_t operator%(const uint256_t& other) const { // оператор остатка по модулю
        if (other == uint256_t(0)) {
            throw std::overflow_error("Modulo by zero");
        }
        uint256_t dividend = *this;
        uint256_t divisor = other;
        uint256_t current = 0;
        for (int i = 255; i >= 0; --i) {
            current = current <<= 1;
            if (dividend.get_bit(i)) {
                current.set_bit(0);
            }

            if (current >= divisor) {
                current = current - divisor;
            }
        }
        return current;
    }

    bool is_zero() const {
        return all_of(values.begin(), values.end(), [](uint32_t v) { return v == 0; });
    }

    // Операторы сравнения
    bool operator==(const uint256_t& other) const {
        return values == other.values;
    }

    bool operator!=(const uint256_t& other) const {
        return !(*this == other);
    }

    bool operator<(const uint256_t& other) const {
        for (int i = 7; i >= 0; --i) {
            if (values[i] != other.values[i]) {
                return values[i] < other.values[i];
            }
        }
        return false;
    }

    bool operator>(const uint256_t& other) const {
        return other < *this;
    }

    bool operator<=(const uint256_t& other) const {
        return !(*this > other);
    }

    bool operator>=(const uint256_t& other) const {
        return !(*this < other);
    }

    uint256_t operator&(const uint256_t& other) const { // Оператор побитового "И"
        uint256_t result;
        for (size_t i = 0; i < 8; ++i) {
            result.values[i] = values[i] & other.values[i];
        }
        return result;
    }

    uint256_t operator|(const uint256_t& other) const { // Оператор побитового "ИЛИ"
        uint256_t result;
        for (size_t i = 0; i < 8; ++i) {
            result.values[i] = values[i] | other.values[i];
        }
        return result;
    }

    uint256_t operator<<=(size_t shift) const { // оператор побитового смещения влево
        uint256_t result;
        size_t block_shift = shift / 32;
        size_t bit_shift = shift % 32;
        if (block_shift >= 8) {
            return result;
        }
        for (size_t i = 7; i >= block_shift && i < 8; --i) {
            result.values[i] = values[i - block_shift] << bit_shift;
            if (bit_shift > 0 && i > block_shift) {
                result.values[i] |= (values[i - block_shift - 1] >> (32 - bit_shift));
            }
        }
        return result;
    }

    uint256_t operator>>=(size_t shift) const { // Оператор  побитового смещения вправо
        uint256_t result;
        size_t block_shift = shift / 32;
        size_t bit_shift = shift % 32;
        if (block_shift >= 8) {
            return result;
        }
        for (size_t i = 0; i + block_shift < 8; ++i) {
            result.values[i] = values[i + block_shift] >> bit_shift;
            if (bit_shift > 0 && i + block_shift + 1 < 8) {
                result.values[i] |= (values[i + block_shift + 1] << (32 - bit_shift));
            }
        }
        return result;
    }

    friend ostream& operator<<(ostream& os, const uint256_t& value) { // оператор вывода
        os << "0x";
        for (int i = 7; i >= 0; --i) {
            os << hex << setw(8) << setfill('0') << value.values[i];
        }
        return os;
    }

private:
    array<uint32_t, 8> values;

    bool get_bit(size_t index) const {
        if (index >= 256) return false;
        return (values[index / 32] & (static_cast<uint32_t>(1) << (index % 32))) != 0;
    }

    void set_bit(size_t index) {
        if (index < 256) {
            values[index / 32] |= (static_cast<uint32_t>(1) << (index % 32));
        }
    }
};
