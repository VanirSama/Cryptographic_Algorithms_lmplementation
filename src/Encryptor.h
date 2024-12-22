#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <cstring>
#include <bitset>
#include "uint256.h"
using namespace std;

#define LB32 0x00000001
#define LB64 0x0000000000000001
#define L64_MASK 0x00000000ffffffff
#define H64_MASK 0xffffffff00000000

inline vector<string> splitIntoBlocks(const string& s) { // разбиение строки на блоки по 8 символов
    vector<string> res;
    for (int i = 0; i < s.length(); i += 8) {
        string sub = s.substr(i, 8);
        res.push_back(sub);
    }
    return res;
}

inline uint64_t BlockToUint64(const string& b) { // конвертация бока в значение uint64_t
    string res = b;
    size_t len = res.size();
    if (len < 8) {
        res.append(8 - len, '\0'); // добавляем нули для дополнения до 8 символов
    }
    uint64_t r = 0;
    for (char c : res) {
        r = (r << 8) | static_cast<uint8_t>(c);
    }
    return r;
}

inline vector<uint64_t> stringToUint64Array(const string& s) { // разбиение строки на массив значений uint64_t
    vector<uint64_t> result;
    vector<string> blocks = splitIntoBlocks(s);
    for (string b : blocks) {
        uint64_t temp = BlockToUint64(b);
        result.push_back(temp);
    }
    return result;
}

inline string uint64ToString(uint64_t num) { // обратный перевод значения uint64_t в строку
    string res;
    string bin = bitset<64>(num).to_string();
    vector<string> temp_bin = splitIntoBlocks(bin);
    for (string b : temp_bin) {
        uint8_t value = 0;
        for (size_t i = 0; i < 8; ++i) {
            if (b[i] == '1') {
                value |= (1 << (7 - i));
            }
        }
        char c = static_cast<char>(value);
        res.append(1, c);
    }
    return res;
}

inline string uint64ArrayToString(const vector<uint64_t>& vals) { // перевод массива uint64_t в строку
    string result;
    for (uint64_t value : vals) {
        string temp = uint64ToString(value);
        result.append(temp);
    }
    return result;
}

inline static bool isPrime(uint64_t prime) { // Проверка простого числа для алгоритма RSA
    uint64_t i, j;
    j = (uint64_t)sqrt((long double)prime);
    for (i = 2; i <= j; i++) {
        if (prime % i == 0) {
            return false;
        }
    }
    return true;
}

inline uint64_t generatePrime() { // Генерация простого числа
    uint64_t prime = (rand() % 1000000 + 10000);
    while (!isPrime(prime)){
        prime++;
    }
    return prime;
}

inline uint256_t modExp(uint256_t base, uint256_t exp, uint256_t mod) { // озведение в степень с остатком по модулю
    uint256_t result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        exp = exp >>= 1;
        base = (base * base) % mod;
    }
    return result;
}

class Encryptor {
protected:
    virtual string algorithm() {
        return 0;
    };

public:
    template <typename T1, typename T2, typename T3, typename T4>
    T1 encrypt(T2 param1, T3 param2, T4 parm3) {
        return 0;
    };

    template <typename T1, typename T2, typename T3, typename T4>
    T1 decrypt(T2 param1, T3 param2, T4 parm3) {
        return 0;
    };

    virtual void demo(char mode) {};
};
