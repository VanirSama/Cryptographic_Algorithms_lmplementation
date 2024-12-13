#include <iostream>
#include <cmath>
#include <string>
#include <cstdint>
#include <vector>
#include <array>
#include "Encryptor.h"
#include "uint256.h"
using namespace std;

class RSA : public Encryptor {
private:
    vector<uint256_t> privateKeyPair = { 0, 0 };

    uint256_t calculateE(uint256_t phi) { // Выбирается целое число e ( 1 < e < phi ) взаимно простое со значением функции Эйлера (phi)
        uint256_t e;
        for (e = 2; e < phi; e++) {
            if (greatestCommonDivisor(e, phi) == 1) {
                return e;
            }
        }
        return -1;
    }

    uint256_t greatestCommonDivisor(uint256_t e, uint256_t phi) { // поиск НОД
        while (e > 0) {
            uint256_t temp = e;
            e = phi % e;
            phi = temp;
        }
        return phi;
    }

    uint256_t calculateD(uint256_t e, uint256_t t) { // Вычисляется число d, мультипликативно обратное к числу e по модулю ф(n)
        uint256_t d, k = 1;
        while (true)
        {
            k = k + t;
            if (k % e == 0) {
                d = (k / e);
                return d;
            }
        }
    }

public:
    RSA() {};
    ~RSA() {};

    void nullifyPrivateKey() { // обнуление пары закрытого ключа
        privateKeyPair = { 0, 0 };
    }

    vector<uint256_t> generateKeyPairs(uint256_t p, uint256_t q) { // создание пары публичного ключа {e, n} и пары закрытого ключа {d, n}
        uint256_t n = p * q, phi = (p - 1) * (q - 1);
        uint256_t e = calculateE(phi), d = calculateD(e, phi);
        vector<uint256_t> publicKeyPair = { e, n };
        privateKeyPair = { d, n };
        return publicKeyPair;
    }

    vector<uint256_t> encrypt(const string& text, uint256_t e, uint256_t n) {
        vector<uint256_t> encryptedText;
        vector<uint8_t> byteArray(text.begin(), text.end());
        for (uint8_t byte: byteArray) {
            uint256_t m = static_cast<uint256_t>(byte);
            uint256_t result = modExp(m, e, n);
            encryptedText.push_back(result);
        }
        return encryptedText;
    }

    string decrypt(const vector<uint256_t>& text) {
        string decryptedText;
        for (const uint256_t& n : text) {
            uint256_t result = modExp(n, privateKeyPair[0], privateKeyPair[1]);
            uint8_t res = 0;
            memcpy(&res, result.ExportValues().data(), sizeof(uint8_t));
            decryptedText.push_back(static_cast<char>(res));
        }
        return decryptedText;
    }

    void printEncryptedString(vector<uint256_t> enc) {
        if (enc.size() > 5) {
            for (size_t i = 0; i < 5; i++) {
                cout << enc[i] << "\n";
            }
            cout << "+ " << dec << (enc.size() - 5) << " values...\n";
        }
        else {
            for (uint256_t val : enc) {
                cout << val << "\n";
            }
        }
    }

    void demo(char mode) { // Демонстрация работы алгоритма
        nullifyPrivateKey();
        bool flag;
        uint64_t p, q;
        cout << "\nEncryption RSA\n-------------------------------------------\n";

        if (mode == 'e') {
            do {
                cout << "Enter a Prime number p : ";
                cin >> p;
                flag = isPrime(p);
                if (flag == false) {
                    cout << "\nInvalid input (" << dec << p << " is not Prime)\n";
                }
                cin.ignore();
            } while (flag == false);

            do {
                cout << "Enter a Prime number q : ";
                cin >> q;
                flag = isPrime(q);
                if (flag == false) {
                    cout << "\nInvalid input (" << dec << q << " is not Prime)\n";
                }
                cin.ignore();
            } while (flag == false);
        }

        if (mode == 'g') {
            p = generatePrime();
            cout << "Prime number p: " << dec << p << "\n";
            q = generatePrime();
            cout << "Prime number q: " << dec << q << "\n";
        }

        vector<uint256_t> publicKey = generateKeyPairs(p, q);
        cout << "\nPublic key is :\ne: " << publicKey[0] << "\nn: " << publicKey[1] << "\n";
        cout << "\nPlain text: ";
        string text;
        getline(cin, text);
        vector<uint256_t> RSAencrypt = encrypt(text, publicKey[0], publicKey[1]);
        cout << "Encrypted text: \n";
        printEncryptedString(RSAencrypt);

        cout << "\nDecryption RSA\n-------------------------------------------\n";
        cout << "Encrypted text: \n";
        printEncryptedString(RSAencrypt);
        cout << "Private key:\ne: ******\nn: ******\n";
        string RSAdecrypt = decrypt(RSAencrypt);
        cout << "Decrypted text: " << RSAdecrypt << "\n";
    }
};
