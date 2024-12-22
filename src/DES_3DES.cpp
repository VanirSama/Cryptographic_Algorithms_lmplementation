#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <iomanip>
#include <cstring>
#include <bitset>
#include "Encryptor.h"
using namespace std;

class DES : public Encryptor {
protected:
    const vector<char> InitialPermutation = { // ìàòðèöà íà÷àëüíîé ïåðåñòàíîâêà áëîêà 64 áèò
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
    };

    const vector<char> ReversePermutation = { // ìàòðèöà îáðàòíîé ïåðåñòàíîâêè áèòîâ
        40, 8, 48, 16, 56, 24, 64, 32,
        39, 7, 47, 15, 55, 23, 63, 31,
        38, 6, 46, 14, 54, 22, 62, 30,
        37, 5, 45, 13, 53, 21, 61, 29,
        36, 4, 44, 12, 52, 20, 60, 28,
        35, 3, 43, 11, 51, 19, 59, 27,
        34, 2, 42, 10, 50, 18, 58, 26,
        33, 1, 41, 9, 49, 17, 57, 25
    };

    const vector<char> ExpansionTable = { // ìàòðèöà ðàñøèðåíèÿ ïîäáëîêà ñ 32 áèò äî 48 áèò
       32, 1, 2, 3, 4, 5,
       4, 5, 6, 7, 8, 9,
       8, 9, 10, 11, 12, 13,
       12, 13, 14, 15, 16, 17,
       16, 17, 18, 19, 20, 21,
       20, 21, 22, 23, 24, 25,
       24, 25, 26, 27, 28, 29,
       28, 29, 30, 31, 32, 1
    };

    const vector<char> Permutation = { // ìàòðèöà ïåðåñòàíîâêè ðàóíäà
        16, 7, 20, 21,
        29, 12, 28, 17,
        1, 15, 23, 26,
        5, 18, 31, 10,
        2, 8, 24, 14,
        32, 27, 3, 9,
        19, 13, 30, 6,
        22, 11, 4, 25
    };

    const vector<vector<char>> STables = { // S-òàáëèöû ïðåîáðàçîâàíèÿ
        {
            // S1
            14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
            0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
            4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
            15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13
        },
        {
            // S2
            15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
            3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
            0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
            13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9
        },
        {
            // S3
            10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
            13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
            13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
            1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12
        },
        {
            // S4
            7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
            13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
            10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
            3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14
        },
        {
            // S5
            2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
            14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
            4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
            11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3
        },
        {
            // S6
            12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
            10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
            9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
            4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13
        },
        {
            // S7
            4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
            13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
            1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
            6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12
        },
        {
            // S8
            13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
            1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
            7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
            2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11
        }
    };
    
    const vector<char> PK1 = { // ìàòðèöà ïåðåñòàíîâêè îñíîâíîãî êëþ÷à äëÿ ãåíåðèðîâàíèÿ êëþ÷åé
        57, 49, 41, 33, 25, 17, 9,
        1, 58, 50, 42, 34, 26, 18,
        10, 2, 59, 51, 43, 35, 27,
        19, 11, 3, 60, 52, 44, 36,
        63, 55, 47, 39, 31, 23, 15,
        7, 62, 54, 46, 38, 30, 22,
        14, 6, 61, 53, 45, 37, 29,
        21, 13, 5, 28, 20, 12, 4
    };

    const vector<char> PK2 = { // Âåêòîð ñãåíåðèðîâàííîãî êëþ÷à
        14, 17, 11, 24, 1, 5,
        3, 28, 15, 6, 21, 10,
        23, 19, 12, 4, 26, 8,
        16, 7, 27, 20, 13, 2,
        41, 52, 31, 37, 47, 55,
        30, 40, 51, 45, 33, 48,
        44, 49, 39, 56, 34, 53,
        46, 42, 50, 36, 29, 32
    };

    const vector<char> IterationShift = { // ëåâûå öèêëè÷åñêèå ñäâèãè êëþ÷à
        1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
    };

    string enc(string input, string key) {
        return algorithm(input, key, 'e');
    }

    string dec(string input, string key) {
        return algorithm(input, key, 'd');
    }

    string generateKey(int length) { // Ãåíåðàöèÿ êëþ÷à
        string key;
        for (int i = 0; i < length; i++) { // ãåíåðàöèÿ êëþ÷à äëèíîé length ñèìâîëîâ
            int rd_n = (rand() % 256);
            if ((rd_n > 31) && (rd_n != 92) && (rd_n != 127)) { // èñêëþ÷åíèå âîçìîæíîñòè äîáàâëåíèÿ ñëóæåáíûõ ñèìâîëîâ è ñèìâîëà '\'
                char c = char(rd_n);
                key.append(1, char(rd_n));
            }
            else { i--; }
        }
        return key;
    }

public:
    DES() {};
    ~DES() {};

    string encrypt(string input, string key) {
        string result = enc(input, key);
        return result;
    };
    string decrypt(string input, string key) {
        string result = dec(input, key);
        return result;
    };

    void demo(char mode) { // Äåìîíñòðàöèÿ ðàáîòû àëãîðèòìà
        string DESresult;
        string DESinput;
        cout << "\nEncryption DES\n-------------------------------------------\n";
        cout << "Plain text: ";
        getline(cin, DESinput);
        string DESkey;
        if (mode == 'g') {
            DESkey = generateKey(7);
            cout << "Generated key: " << DESkey << endl;
        }

        if (mode == 'e') {
            bool flag;
            do {
                cout << "Key [7 characters]: ";
                getline(cin, DESkey);
                flag = (DESkey.size() == 7);
                if (flag == false) {
                    cout << "\nInvalid input (Key should be 7 characters long (56 bits))\n";
                }
            } while (flag == false);
            cout << "Given key: " << DESkey << endl;
        }

        DESresult = encrypt(DESinput, DESkey);
        cout << "Encrypted text: " << DESresult << "\n";

        cout << "\nDecryption DES\n-------------------------------------------\n";
        cout << "Encrypted text: " << DESresult << "\n";
        string DESkey_2;
        bool flag;

        do {
            cout << "Key [7 characters]: ";
            getline(cin, DESkey_2);
            flag = (DESkey_2.size() == 7);
            if (flag == false) {
                cout << "\nInvalid input (Key should be 7 characters long (56 bits))\n";
            }
        } while (flag == false);

        DESresult = decrypt(DESresult, DESkey_2);
        cout << "Decrypted text: " << DESresult << "\n";
    }

private:
    string algorithm(string input, string key, char mode) { // îñíîâíîé àëãîðèòì
        vector<uint64_t> res;
        vector<uint64_t> blocks = stringToUint64Array(input);
        uint64_t k = BlockToUint64(key);
        for (uint64_t b : blocks)
        {
            int i, j;

            char line, column;
            uint32_t C = 0, D = 0;
            uint32_t L = 0, R = 0, s_output = 0, f_function_res = 0, temp = 0;
            uint64_t sub_key[16] = { 0 };
            uint64_t s_input = 0;
            uint64_t permuted_choice_1 = 0, permuted_choice_2 = 0;
            uint64_t init_perm_res = 0, inv_init_perm_res = 0, pre_output = 0;

            // Èçíà÷àëüíàÿ ïåðåñòàíîâêà
            for (i = 0; i < 64; i++) {
                init_perm_res <<= 1;
                init_perm_res |= (b >> (64 - InitialPermutation[i])) & LB64;
            }

            // Ïåðåñòàíîâêà ïðàâîé è ëåâîé ïîëîâèíû áëîêîâ
            L = (uint32_t)(init_perm_res >> 32) & L64_MASK;
            R = (uint32_t)init_perm_res & L64_MASK;

            // Ïåðåñòàíîâêà êëþ÷à
            for (i = 0; i < 56; i++) {
                permuted_choice_1 <<= 1;
                permuted_choice_1 |= (k >> (64 - PK1[i])) & LB64;
            }

            // Ãåíåðèðîâàíèå êëþ÷åé è ñîçäàíèå áëîêîâ C è D
            C = (uint32_t)((permuted_choice_1 >> 28) & 0x0fffffff);
            D = (uint32_t)(permuted_choice_1 & 0x0fffffff);

            for (i = 0; i < 16; i++) {
                for (j = 0; j < IterationShift[i]; j++) {
                    C = (0x0fffffff & (C << 1)) | (0x00000001 & (C >> 27));
                    D = (0x0fffffff & (D << 1)) | (0x00000001 & (D >> 27));
                }

                permuted_choice_2 = (((uint64_t)C) << 28) | (uint64_t)D;
                sub_key[i] = 0;
                for (j = 0; j < 48; j++) {
                    sub_key[i] <<= 1;
                    sub_key[i] |= (permuted_choice_2 >> (56 - PK2[j])) & LB64;
                }
            }

            //Ôóíêöèÿ Ôåéñòåëÿ
            for (i = 0; i < 16; i++) {
                s_input = 0;

                for (j = 0; j < 48; j++) {
                    s_input <<= 1;
                    s_input |= (uint64_t)((R >> (32 - ExpansionTable[j])) & LB32);
                }

                if (mode == 'd') {
                    s_input ^= sub_key[15 - i];
                }
                else {
                    s_input ^= sub_key[i];
                }

                for (j = 0; j < 8; j++) {
                    line = (char)((s_input & (0x0000840000000000 >> 6 * j)) >> (42 - 6 * j));
                    line = (line >> 4) | (line & 0x01);

                    column = (char)((s_input & (0x0000780000000000 >> 6 * j)) >> (43 - 6 * j));

                    s_output <<= 4;
                    s_output |= (uint32_t)(STables[j][16 * line + column] & 0x0f);
                }

                f_function_res = 0;
                // ïåðåñòàíîâêà
                for (j = 0; j < 32; j++) { 
                    f_function_res <<= 1;
                    f_function_res |= (s_output >> (32 - Permutation[j])) & LB32;
                }

                // ïåðåñòàíîâêà Ôåéñòåëÿ
                temp = R;
                R = L ^ f_function_res;
                L = temp;
            }

            // êîìáèíàöèÿ L è R áëîêîâ
            pre_output = (((uint64_t)R) << 32) | (uint64_t)L;

            for (i = 0; i < 64; i++) { // îáðàòíàÿ ïåðåñòàíîâêà
                inv_init_perm_res <<= 1;
                inv_init_perm_res |= (pre_output >> (64 - ReversePermutation[i])) & LB64;
            }
            res.push_back(inv_init_perm_res);
        }
        string result = uint64ArrayToString(res);
        return result;
    }
};

// íàñëåäîâàíèå êëàññà DES êëàññîì TripleDES
class TripleDES : public DES {
public:
    TripleDES() {};
    ~TripleDES() {};

    string encrypt(string input, string key) {

        // ðàçáèòèå êëþ÷à íà òðè 64-áèòíûõ áëîêà
        vector<uint64_t> keys = stringToUint64Array(key);

        uint64_t key1 = keys[0];
        uint64_t key2 = keys[1];
        uint64_t key3 = keys[2];

        // ïðåîáðàçîâàíèå uint64_t îáðàòíî â ñòðîêó äëÿ èñïîëüçîâàíèÿ â DES
        string key1_str = uint64ToString(key1);
        string key2_str = uint64ToString(key2);
        string key3_str = uint64ToString(key3);

        // ïîñëåäîâàòåëüíîå øèôðîâàíèå
        string result = DES::enc(input, key1_str);
        result = DES::dec(result, key2_str);
        result = DES::enc(result, key3_str);
        return result;
    }

    string decrypt(string input, string key) {

        // ðàçáèòèå êëþ÷à íà òðè 64-áèòíûõ áëîêà
        vector<uint64_t> keys = stringToUint64Array(key);

        uint64_t key1 = keys[0];
        uint64_t key2 = keys[1];
        uint64_t key3 = keys[2];

        // ïðåîáðàçîâàíèå uint64_t îáðàòíî â ñòðîêó äëÿ èñïîëüçîâàíèÿ â DES
        string key1_str = uint64ToString(key1);
        string key2_str = uint64ToString(key2);
        string key3_str = uint64ToString(key3);

        // ïîñëåäîâàòåëüíîå äåøèôðîâàíèå
        string result = DES::dec(input, key3_str);
        result = DES::enc(result, key2_str);
        result = DES::dec(result, key1_str);
        return result;
    }

    void demo(char mode) { // Äåìîíñòðàöèÿ ðàáîòû àëãîðèòìà
        string TDESresult;
        string TDESinput;
        cout << "\nEncryption 3-DES\n-------------------------------------------\n";
        cout << "Plain text: ";
        getline(cin, TDESinput);
        string TDESkey;

        if (mode == 'g') {
            TDESkey = generateKey(21);
            cout << "Generated key: " << TDESkey << endl;
        }

        if (mode == 'e') {
            bool flag;
            do {
                cout << "Key [21 characters]: ";
                getline(cin, TDESkey);
                flag = (TDESkey.size() == 21);
                if (flag == false) {
                    cout << "\nInvalid input (Key should be 21 characters long (168 bits))\n";
                }
            } while (flag == false);
            cout << "Given key: " << TDESkey << endl;
        }


        TDESresult = encrypt(TDESinput, TDESkey);
        cout << "Encrypted text: " << TDESresult << "\n";

        cout << "\nDecryption 3-DES\n-------------------------------------------\n";
        cout << "Encrypted text: " << TDESresult << "\n";
        string TDESkey_2;
        bool flag;

        do {
            cout << "Key [21 characters]: ";
            getline(cin, TDESkey_2);
            flag = (TDESkey_2.size() == 21);
            if (flag == false) {
                cout << "\nInvalid input (Key should be 21 characters long (168 bits))\n";
            }
        } while (flag == false);

        TDESresult = decrypt(TDESresult, TDESkey_2);
        cout << "Decrypted text: " << TDESresult << "\n";
    }
};
