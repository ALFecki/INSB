#include <httplib.h>
#include <des.h>
#include "utils.h"
#include <iostream>
#include <chrono>

using namespace httplib;

using namespace std;

constexpr const char* key = "serverkc";

int main() {
    Server svc;

    DES c;
    c.setKey(key);

    svc.Post("/hello", [&](const Request& request, Response& response) {
        cout << "POST body from client to SS: " << request.body << endl;

        auto tgs = split(c.decryptAnyString(split(request.body, ';').front()), ';');

        string c_ss = tgs.back();
        cout << "C_SS from client: " << c_ss << endl;
        string client_id = tgs.front();

        cout << "Received tgs from client '" << client_id << "': " << endl;

        for (const auto& cur : tgs) {
            cout << cur << endl;
        }

        c.setKey(c_ss);

        auto aut = split(c.decryptAnyString(split(request.body, ';').back()), ';');

        cout << "Received aut from client: " << endl;

        for (const auto& cur : aut) {
            cout << "!!!" << endl;
            cout << cur << endl;
        }
        std::cout << client_id << endl;
        cout << string(aut.front()) << endl;
        
        if (client_id != string(aut.front())) {
            response.status = 403;
            const char* resp = "Forbidden (incorrect user)";
            response.set_content(resp, strlen(resp), "text/plain");
            return;
        }

        string answer = "";

        response.status = 200;
        response.body = c.encryptAnyString(to_string(std::atoll(aut.back().c_str()) + 1));
    });

    svc.listen("0.0.0.0", 8081);

    return 0;
}
