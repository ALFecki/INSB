#include "httplib.h"
#include <map>
#include <iostream>
#include <chrono>
#include "des/des.h"
#include "utils.h"

using namespace httplib;

using namespace std;

using namespace chrono_literals;

struct ClientInfo {
    string key;
    string tgs_key;
    string c_ss;
    vector<string> allowed_servers;
};

struct ServerInfo {
    string tgs_key;
};

const string MASTER_PASSWORD = "password";
const string TGS = "authoritive_server";

map<string, ClientInfo> clients = {
    {"newusername", {
                        .key = "password",
                        .tgs_key = "mysuper2",
                        .c_ss = "internal",
                        .allowed_servers = {
                            "SS",
                        },
                    }},
};

map<string, ServerInfo> servers = {
    {"SS", {
                   .tgs_key = "serverkc",
               }}};

void initMessage(const Request& request, Response& response) {
    string id = request.path_params.at("id");

    if (clients.find(id) == clients.end()) {
        response.status = 403;
        response.set_content("Forbidden", 10, "text/plain");
        return;
    }

    cout << "STEP 1: Client '" << id << "' requested to auth" << endl;

    DES c;

    auto current_time = chrono::system_clock::now();
    auto end_time = current_time + 24h;

    string TGT = id + ";" + TGS + ";" + to_string(chrono::system_clock::to_time_t(current_time)) + ";" + to_string(chrono::system_clock::to_time_t(end_time)) + ";" + clients[id].key;

    c.setKey(MASTER_PASSWORD);

    string TGT_encrypted = c.encryptAnyString(TGT);

    string answer = TGT_encrypted + ";" + clients[id].tgs_key;

    c.setKey(clients[id].key);

    cout << "Message for client before encrypting: " << answer << endl;

    string encrypted = c.encryptAnyString(answer);

    response.set_content(encrypted.c_str(), encrypted.size(), "text/plain");
    response.status = 200;
}

void grant(const Request& request, Response& response) {
    string id = request.path_params.at("id");

    if (clients.find(id) == clients.end()) {
        response.status = 403;
        response.set_content("Forbidden", 10, "text/plain");
        return;
    }

    auto data = split(request.body, ';');

    cout << "STEP 3: Data from client '" << id << "':" << endl;

    for (const auto& cur : data) {
        cout << cur << endl;
    }

    DES c;
    c.setKey(clients[id].tgs_key);

    auto aut = split(c.decryptAnyString(data[1]), ';');

    cout << "STEP 3: Aut from client: name='" << id << "' time='" << aut.back() << "'" << endl;

    string server_id = data.back().c_str();

    if (
        find(clients[id].allowed_servers.begin(), clients[id].allowed_servers.end(), server_id) == clients[id].allowed_servers.end() ||
        aut.front().substr(0, 8) == id) {
        response.status = 403;
        const char* resp = "Forbidden (incorrect user)";
        response.set_content(resp, strlen(resp), "text/plain");
        return;
    }

    auto ticket_creation = chrono::system_clock::from_time_t(std::atoll(aut.back().c_str()));

    c.setKey(MASTER_PASSWORD);

    auto tgt = split(c.decryptAnyString(data.front()), ';');

    cout << "STEP 3: TGT from client '" << id << "':" << endl;

    for (const auto& cur : tgt) {
        cout << cur << endl;
    }

    if (
        chrono::system_clock::from_time_t(std::atoll(tgt[2].c_str())) > ticket_creation ||
        chrono::system_clock::from_time_t(std::atoll(tgt[3].c_str())) < ticket_creation) {
        response.status = 403;
        const char* resp = "Forbidden (ticket expired)";
        response.set_content(resp, strlen(resp), "text/plain");
        return;
    }

    c.setKey(servers[server_id].tgs_key);

    auto current_time = chrono::system_clock::now();
    auto end_time = current_time + 24h;

    string tgs_server = id + ";" + server_id + ";" + to_string(chrono::system_clock::to_time_t(current_time)) + ";" + to_string(chrono::system_clock::to_time_t(end_time)) + ";" + clients[id].c_ss;

    string tgs_server_encrypted = c.encryptAnyString(tgs_server);

    string answer = tgs_server_encrypted + ";" + clients[id].c_ss;

    cout << "Response to '" << id << "' before encrypting: " << answer << endl;

    c.setKey(clients[id].tgs_key);

    string answer_encrypted = c.encryptAnyString(answer);

    response.body = answer_encrypted;
    response.status = 200;
}

int main() {
    Server svc;

    svc.Get("/init/:id", initMessage);

    svc.Post("/grant/:id", grant);

    svc.listen("0.0.0.0", 8080);

    return 0;
}
