#include <iostream>
#include "des/des.h"
#include "httplib.h"
#include "utils.h"
#include <chrono>
#include <termios.h>


using namespace std;

using namespace httplib;

constexpr const char* server = "SS";

void disableEcho() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void enableEcho() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}


int main() {
    Client cli("http://127.0.0.1:8080");

    string username, password;
    
    cout << "Enter login: ";
    cin >> username;
    
    char ch;
    disableEcho();

    cout << "Enter password: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, password);
    cout << endl;

    enableEcho();

    auto response = cli.Get("/init/" + username);

    if (response->status != 200) {
        cout << "Auth server answered with " << response->status << " code" << endl;
        return -1;
    }

    string resp = response->body;

    DES c;
    c.setKey(password);

    string result = c.decryptAnyString(resp);

    cout << "STEP 2: Auth server response: " << result << endl;

    string tgs_key = split(result, ';').back().substr(0, 8);

    string aut = username + ";" + to_string(chrono::system_clock::to_time_t(chrono::system_clock::now()));

    c.setKey(tgs_key);

    string aut_encrypted = c.encryptAnyString(aut);

    string request_to_tgs = split(result, ';').front() + ";" + aut_encrypted + ";" + server;

    response = cli.Post("/grant/" + username, request_to_tgs, "plain/text");

    if (response->status != 200) {
        cout << "Auth server answered with non 200 code: " << response->status << endl;
        return -1;
    }

    result = c.decryptAnyString(response->body);

    cout << "STEP 4: Auth server response: " << result << endl;

    Client hello("http://localhost:8081");

    auto tgs = split(result, ';').front();

    auto current_time = chrono::system_clock::to_time_t(chrono::system_clock::now());

    aut = username + ";" + to_string(current_time);

    cout << "Aut before encoding: " << aut << endl;

    c.setKey(string(split(result, ';').back()));

    auto request_to_server = tgs + ";" + c.encryptAnyString(aut);

    response = hello.Post("/hello", request_to_server, "plain/text");

    if (response->status != 200) {
        cout << "Server answered with non 200 code: " << response->status << endl;
        return -1;
    }

    auto answer = split(c.decryptAnyString(response->body), ';');
    
    if (std::atoll(answer.back().c_str()) != current_time + 1) {
        cout << "Server verification failed!" << endl;
        return 1;
    }

    cout << "Server authetication succeed" << endl;
    cout << "Server responded with: " << answer.front() << endl;

    return 0;
}
