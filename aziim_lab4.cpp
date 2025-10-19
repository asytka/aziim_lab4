#include <iostream>
#include <unordered_set>
#include <fstream>
#include <string> 
#include <vector>

using namespace std;

bool error = false;
const string alph = "абвгґдеєжзиіїйклмнопрстуфхцчшщьюяАБВГҐДЕЄЖЗИІЇЙКЛМНОПРСТУФХЦЧШЩЬЮЯ";

vector<int> codify(string text) {
	vector<int> res;
	for (const auto& c : text) {
		int code = alph.find(c);
		if (code != string::npos) {
			res.push_back(code + 1);
		}
	}
	return res;
}

string uncodify(vector<int> code) {
	string res;
	for (const auto& i : code) {
		res += alph[i - 1];
	}
	return res;
}

string encrypt(string cleartext, string key) {
	vector<int> cleartext_code = codify(cleartext);
	vector<int> key_code = codify(key);
	vector<int> encrypted_code(cleartext_code.size());

	if (key_code.empty()) {
		cout << "Please enter correct key!\n";
		error = true;
		return " ";
	}

	for (int i = 0; i < cleartext_code.size(); i++) {
		encrypted_code[i] = (cleartext_code[i] + key_code[i % key_code.size()] - 1) % alph.size() + 1;
	}
	return uncodify(encrypted_code);
}

string decrypt(vector<int> enctext, vector<int> key) {
	vector<int> decrypted_code(enctext.size());

	for (int i = 0; i < enctext.size(); i++) {
		decrypted_code[i] = (enctext[i] - key[i % key.size()] + alph.size() - 1) % alph.size() + 1;
	}

	return uncodify(decrypted_code);
}

int main() {
	ifstream inputFile("input.txt");
	ofstream encFile("encrypted.txt");
	ofstream decFile("decryped.txt");
	inputFile.imbue(locale("uk_UA.utf8"));
	encFile.imbue(locale("uk_UA.utf8"));
	decFile.imbue(locale("uk_UA.utf8"));
	string key;
	cout << "Please enter an encrpytion key: ";
	getline(cin, key);

	string input;
	getline(inputFile, input);
	string encrypted = encrypt(input, key);
	if (error) return 1;
	encFile << encrypted;
	string decrypted = decrypt(codify(encrypted), codify(key));
 	decFile << decrypted;
	cout << "Encryption and decryption of the file is done!\n";
	return 0;
}