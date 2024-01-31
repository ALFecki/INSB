#include <iostream>
#include <fstream>

std::string encryptVigenere(std::string text, std::string keyword, bool encrypt = true) {
    std::string result = "";
    int keywordIndex = 0;
    for (int i = 0; i < text.length(); i++) {
        if (isalpha(text[i])) {
            char base = isupper(text[i]) ? 'A' : 'a';
            char offset = isupper(keyword[keywordIndex]) ? 'A' : 'a';
            int messageIndex = text[i] - base;
            int keyCharIndex = keyword[keywordIndex] - offset;
            if (encrypt) {
                result += static_cast<char>((messageIndex + keyCharIndex) % 26 + base);
            } else {
                result += static_cast<char>((messageIndex - keyCharIndex + 26) % 26 + base);
            }
            keywordIndex = (keywordIndex + 1) % keyword.length();
        } else {
            result += text[i];
        }
    }
    return result;
}

std::string decryptVigenere(std::string text, std::string keyword) {
    return encryptVigenere(text, keyword, false);
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