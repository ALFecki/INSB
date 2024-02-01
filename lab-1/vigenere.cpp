#include <iostream>
#include <fstream>
#include <tuple>

std::tuple<char, int, int> getBaseParameters(std::string keyword, char textChar, int keywordIndex) {
    char base = isupper(textChar) ? 'A' : 'a';
    char offset = isupper(keyword[keywordIndex]) ? 'A' : 'a';
    int messageIndex = textChar - base;
    int keyCharIndex = keyword[keywordIndex] - offset;
    return std::tuple<char, int, int>(base, messageIndex, keyCharIndex);
}

std::string encryptVigenere(std::string text, std::string keyword) {
    std::string result = "";
    int keywordIndex = 0;
    for (auto& ch: text) {
        if (isalpha(ch)) {
            auto baseParameters = getBaseParameters(keyword, ch, keywordIndex);
            result += static_cast<char>((std::get<1>(baseParameters) + std::get<2>(baseParameters)) % 26 + std::get<0>(baseParameters));
            keywordIndex = (keywordIndex + 1) % keyword.length();
        } else {
            result += ch;
        }
    }
    return result;
}

std::string decryptVigenere(std::string text, std::string keyword) {
    std::string result = "";
    int keywordIndex = 0;
    for (auto& ch: text) {
        if (isalpha(ch)) {
            auto baseParameters = getBaseParameters(keyword, ch, keywordIndex);
            result += static_cast<char>((std::get<1>(baseParameters) - std::get<2>(baseParameters) +26) % 26 + std::get<0>(baseParameters));
            keywordIndex = (keywordIndex + 1) % keyword.length();
        } else {
            result += ch;
        }
    }
    return result;
}

int main() {

    std::string inputFileName, outputFileName, inputText, keyword;

    std::cout << "VIGENERE Cipher for alphabet \"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz\"" << std::endl;
    std::cout << "Enter input file name: ";
    std::getline(std::cin, inputFileName);
    std::cout << "Enter output file name: ";
    std::getline(std::cin, outputFileName);

    std::cout << "Enter keyword: ";
    std::getline(std::cin, keyword);

    std::ifstream inputFile(inputFileName);
    std::ofstream outputFile(outputFileName);
    
    if (inputFile.is_open()) {
        std::getline(inputFile, inputText);
        inputFile.close();

        std::string encryptedText = encryptVigenere(inputText, keyword);
        std::string decryptedText = decryptVigenere(encryptedText, keyword);

        outputFile << "Encrypted text: " << encryptedText << std::endl;
        outputFile << "Decrypted text: " << decryptedText << std::endl;

        outputFile.close();
    } else {
        std::cout << "Unable to open the input file." << std::endl;
    }
    
    return 0;
}