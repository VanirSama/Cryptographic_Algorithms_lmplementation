#include <iostream>
#include <stdio.h>
#include "DES_3DES.cpp"
#include "RSA.cpp"
using namespace std;

class App {
public:
    App() {};
    ~App() {};

    void run() {
        char choice;
        cout << "\n------- Welcome to cryptographic encryption algorithms C++ implementations showcase -------\n";
        while (runningApp) {
            cout << "\n                              ------- Main menu: -------\n";
            displayMenu();
            cin >> choice;
            cin.ignore();

            switch (choice) {
            case '1':
                desMenu();
                break;
            case '2':
                tripleDesMenu();
                break;
            case '3':
                rsaMenu();
                break;
            case '4':
                cout << "\nSee you next time...\n";
                
                runningApp = false;
                return;
            case '?':
                cout << "\n(i) Author: Bakulin Alexei, PIb-2310a, TSU, Togliatti\n\
    Date: 07-12-2024\n\
    Usage: Showcase of cryptographic encryption algorithms\n";
                break;
            default:
                cout << "\n(!) Invalid option. Please, try again\n";
            }
        }
    }

private:
    DES des;
    TripleDES tdes;
    RSA rsa;

    bool runningApp = true;

    void displayMenu() {
        cout << "\n> 1. DES\n";
        cout << "> 2. Triple DES\n";
        cout << "> 3. RSA\n";
        cout << "> 4. Exit\n";
        cout << "> ?. Info about program\n";
        cout << "\nChoose an option: > ";
    }

    void desMenu() {
        bool desrun = true;
        while (desrun) {
            char option;
            cout << "\n                      ------- DES cryptography algorithm -------\n";
            cout << "\n> 1. Enter key\n";
            cout << "> 2. Generate key\n";
            cout << "> 3. Exit to main menu\n";
            cout << "> ?. Info about algorithm\n";
            cout << "\nChoose an option: > ";
            cin >> option;
            cin.ignore();

            switch (option) {
            case '1':
                des.demo('e'); // Âûçîâ àëãîðèòìà DES ñ ââåäåíèåì êëþ÷à
                break;
            case '2':
                des.demo('g'); // Âûçîâ àëãîðèòìà DES ñî ñãåíåðèðîâàííûì êëþ÷îì
                break;
            case '3':
                desrun = false;
                return;
            case '?':
                cout << "\n(i) DES is a symmetric block encryption algorithm that uses a 7-character (56-bit) key to encrypt and decrypt a block of data by combining\n\
    and permuting the bits of the key and block.\n";
                break;
            default:
                cout << "\n(!) Invalid option. Please, try again\n";
            }
        }
    }
    void tripleDesMenu() {
        bool tdesrun = true;
        while (tdesrun) {
            cout << "\n                     ------- 3-DES cryptography algorithm -------\n";
            char option;
            cout << "\n> 1. Enter key\n";
            cout << "> 2. Generate key\n";
            cout << "> 3. Exit to main menu\n";
            cout << "> ?. Info about algorithm\n";
            cout << "\nChoose an option: > ";
            cin >> option;
            cin.ignore();

            switch (option) {
            case '1':
                tdes.demo('e'); // Âûçîâ àëãîðèòìà Triple DES ñ ââåäåíèåì êëþ÷à
                break;
            case '2':
                tdes.demo('g'); // Âûçîâ àëãîðèòìà Triple DES ñî ñãåíåðèðîâàííûì êëþ÷îì
                break;
            case '3':
                tdesrun = false;
                return;
            case '?':
                cout << "\n(i) Triple-DES (3-DES) is a symmetric block encryption algorithm based on the DES algorithm that uses a 21-character (168-bit) key, which is split\n\
    into 3 keys of 7 characters (56 bits) each to perform alternate encryption and decryption of each block using the DES algorithm.\n";
                break;
            default:
                cout << "\n(!) Invalid option. Please, try again\n";
            }
        }
    }
    void rsaMenu() {
        bool rsarun = true;
        
        while (rsarun) {
            cout << "\n                      ------- RSA cryptography algorithm -------\n";
            char option;
            cout << "\n> 1. Enter prime numbers\n";
            cout << "> 2. Generate prime numbers\n";
            cout << "> 3. Exit to main menu\n";
            cout << "> ?. Info about algorith\n";
            cout << "\nChoose an option: > ";
            cin >> option;
            cin.ignore();

            switch (option) {
            case '1':
                rsa.demo('e'); // Âûçîâ àëãîðèòìà RSA ñ ââåäåíèåì ïðîñòûõ ÷èñåë
                break;
            case '2':
                rsa.demo('g'); // Âûçîâ àëãîðèòìà RSA ñî ñãåíåðèðîâàííûìè ïðîñòûìè ÷èñëàìè
                break;
            case '3':
                rsarun = false;
                return;
            case '?':
                cout << "\n(i) RSA is an asymmetric encryption algorithm that uses two keys: public and private. The algorithm is based on the difficulty of factoring the\n\
    product of two large prime numbers\n";
                break;
            default:
                cout << "\n(!) Invalid option. Please, try again\n";
            }
        }
    }
};

int main() {
    srand(static_cast<unsigned int>(time(nullptr))); // Èíèöèàëèçàöèÿ srand äëÿ ãåíåðàöèè ñëó÷àéíûõ ÷èñåë
    App app;
    app.run();
    system("pause");
    return 0;
}
