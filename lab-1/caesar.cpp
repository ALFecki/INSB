#include <iostream>
#include <fstream>
#include <string>
#include <locale.h>

std::string encryptCaesar(std::string text, int shift) {
    std::string result = "";
    int length = text.length();

    for (int i = 0; i < length; ++i) {
        if (isalpha(text[i])) {
            char ch = isupper(text[i]) ? 'A' : 'a';
            result += (text[i] - ch + shift) % 26 + ch;
        } else {
            result += text[i];
        }
    }

    return result;
}

std::string decryptCaesar(std::string text, int shift) {
    return encryptCaesar(text, 26 - shift);
}

int main() {

    std::string inputFileName, outputFileName, inputText;

    std::cout << "CAESAR Cipher for alphabet \"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz\"" << std::endl;
    std::cout << "Enter input file name: ";
    std::getline(std::cin, inputFileName);
    std::cout << "Enter output file name: ";
    std::getline(std::cin, outputFileName);

    std::ifstream inputFile(inputFileName);
    std::ofstream outputFile(outputFileName);
    
    if (inputFile.is_open()) {
        std::getline(inputFile, inputText);
        inputFile.close();

        int shift = 3;

        std::string encryptedText = encryptCaesar(inputText, shift);
        std::string decryptedText = decryptCaesar(encryptedText, shift);

        outputFile << "Encrypted text: " << encryptedText << std::endl;
        outputFile << "Decrypted text: " << decryptedText << std::endl;

        outputFile.close();
    } else {
        std::cout << "Unable to open the input file." << std::endl;
    }
    
    return 0;
}