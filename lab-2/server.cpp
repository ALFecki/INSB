#include "httplib.h"
#include "des/des.h"
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

        string client_id = tgs.front();

        cout << "STEP 5: TGS from client '" << client_id << "': " << endl;
        cout << "ss=" << tgs[1] << endl;
        cout << "t=" << tgs[2] << endl;
        cout << "p=" << tgs[3] << endl;
        cout << "Kc_ss=" << tgs[4] << endl;

        c.setKey(c_ss);

        auto aut = split(c.decryptAnyString(split(request.body, ';').back()), ';');

        cout << "STEP 5: Aut from client: name='" << aut.front() << "' time='" << aut.back() << "'" << endl;

        if (client_id != string(aut.front())) {
            response.status = 403;
            const char* resp = "Forbidden (incorrect user)";
            response.set_content(resp, strlen(resp), "text/plain");
            return;
        }

        cout << "Success client authorization!" << endl;
        
        string answer = "HELLO, WORLD!";

        response.status = 200;
        response.body = c.encryptAnyString(answer + ';' + to_string(std::atoll(aut.back().c_str()) + 1));
    });

    svc.listen("0.0.0.0", 8081);

    return 0;
}
