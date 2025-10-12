#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <locale>
#include <codecvt>
#include <unordered_map>
#include <Windows.h>

using namespace std;

const wstring UA_ALPHABET = L"АБВГҐДЕЄЖЗИІЇЙКЛМНОПРСТУФХЦЧШЩЬЮЯ"
L"абвгґдеєжзииіїйклмнопрстуфхцчшщьюя";

unordered_map<wchar_t, int> charToIndex;
vector<wchar_t> indexToChar;

void initAlphabet() {
    for (size_t i = 0; i < UA_ALPHABET.size(); ++i) {
        charToIndex[UA_ALPHABET[i]] = static_cast<int>(i);
        indexToChar.push_back(UA_ALPHABET[i]);
    }
}

wstring readFile(const string& filename) {
    wifstream file(filename);
    file.imbue(locale(locale(), new codecvt_utf8<wchar_t>));
    wstring content((istreambuf_iterator<wchar_t>(file)), istreambuf_iterator<wchar_t>());
    return content;
}

void writeFile(const string& filename, const wstring& content) {
    wofstream file(filename);
    file.imbue(locale(locale(), new codecvt_utf8<wchar_t>));
    file << content;
}

wstring generateGamma(const wstring& key, size_t length) {
    wstring gamma;
    gamma.reserve(length);
    for (size_t i = 0; i < length; ++i) {
        gamma.push_back(key[i % key.size()]);
    }
    return gamma;
}

wstring process(const wstring& text, const wstring& gamma, bool encrypt) {
    wstring result;
    result.reserve(text.size());
    int N = (int)UA_ALPHABET.size();

    for (size_t i = 0; i < text.size(); ++i) {
        auto itText = charToIndex.find(text[i]);
        auto itGamma = charToIndex.find(gamma[i]);

        if (itText == charToIndex.end() || itGamma == charToIndex.end()) {
            result.push_back(text[i]);
            continue;
        }

        int M = itText->second;
        int G = itGamma->second;
        int C;

        if (encrypt) {
            C = (M + G) % N;
        }
        else {
            C = (M - G + N) % N;
        }

        result.push_back(indexToChar[C]);
    }

    return result;
}

int main() {
    locale::global(locale(""));
    initAlphabet();
    setlocale(LC_CTYPE, "ukr");
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    int choice;
    wstring key;
    string inputFile = "input.txt";
    string encryptedFile = "encrypted.txt";
    string decryptedFile = "decrypted.txt";

    while (true) {
        cout << "\n==== Шифр модульного гамування ====\n";
        cout << "1. Зашифрувати файл\n";
        cout << "2. Розшифрувати файл\n";
        cout << "0. Вийти\n";
        cout << "Ваш вибір: ";
        cin >> choice;
        cin.ignore();

        if (choice == 0) break;

        cout << "Введіть гамму (парольну фразу): ";
        getline(wcin, key);
        if (key.empty()) {
            cout << "Помилка! Введіть не порожній ключ!\n";
            continue;
        }
        if (choice == 1) {
            wstring text = readFile(inputFile);
            wstring gamma = generateGamma(key, text.size());
            wstring encrypted = process(text, gamma, true);
            writeFile(encryptedFile, encrypted);
            cout << "Файл зашифровано у " << encryptedFile.c_str() << endl;
        }
        else if (choice == 2) {
            wstring text = readFile(encryptedFile);
            wstring gamma = generateGamma(key, text.size());
            wstring decrypted = process(text, gamma, false);
            writeFile(decryptedFile, decrypted);
            cout << "Файл розшифровано у " << decryptedFile.c_str() << endl;
        }
        else {
            cout << "Нерозпізнана команда. Спробуйте ще раз:\n";
            continue;
        }
    }

    return 0;
}
