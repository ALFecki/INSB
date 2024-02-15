#include <locale.h>

#include <fstream>
#include <iostream>
#include <limits>
#include <string>

std::string encryptCaesar(std::string text, int shift) {
    std::string result = "";
    int length = text.length();

    for (auto& c : text) {
        if (isalpha(c)) {
            char ch = isupper(c) ? 'A' : 'a';
            result += (c - ch + shift) % 26 + ch;
        } else {
            result += c;
        }
    }

    return result;
}

std::string decryptCaesar(std::string text, int shift) {
    return encryptCaesar(text, 26 - shift);
}

int main() {
    std::string inputFileName = "input.txt", outputFileName = "output.txt", inputText;

    std::cout << "CAESAR Cipher for alphabet \"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz\""
              << std::endl;

    std::cout << "Enter input file name (default: input.txt): ";
    std::string temp;
    std::getline(std::cin, temp);
    if (!temp.empty()) {
        inputFileName = temp;
        temp.clear();
    }
    std::cout << "Enter output file name (default: output.txt): ";
    std::getline(std::cin, temp);
    if (!temp.empty()) {
        outputFileName = temp;
        temp.clear();
    }

    std::ifstream inputFile(inputFileName);
    std::ofstream outputFile(outputFileName);

    if (inputFile.is_open()) {
        inputText =
            std::string((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
        inputFile.close();
    } else {
        std::cout << "Unable to open the input file." << std::endl;
        return 0;
    }

    while (true) {
        int shift = 3;
        std::cout << "Enter shift: ";
        if (!(std::cin >> shift)) {
            std::cout << "Enter correct shift!" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        std::string encryptedText = encryptCaesar(inputText, shift);
        std::string decryptedText = decryptCaesar(encryptedText, shift);

        outputFile << "Encrypted text:\n" << encryptedText << std::endl << std::endl;
        outputFile << "Decrypted text:\n" << decryptedText << std::endl;

        outputFile.close();
        break;
    }

    return 0;
}