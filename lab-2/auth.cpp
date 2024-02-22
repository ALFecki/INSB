#include <des/des.h>

#include <chrono>

#include "httplib.h"

using namespace std::chrono_literals;

const std::string PASSWORD = "testpassword";
const std::string TGS = "authorize_server";

struct ClientInfo {
    std::string key;
    std::string tgs;
    std::string c_ss;
    std::vector<std::string> allowed_servers;
};

struct ServerInfo {
    std::string tgs;
};

std::map<std::string, ClientInfo> clients = {
    {"alf", {.key = "test1", .tgs = "test2", .c_ss = "internal", .allowed_servers = {"testserver"}}}};

std::map<std::string, ServerInfo> servers = {{"testserver",
                                              {
                                                  .tgs = "serverkc",
                                              }}};

void initMessage(const httplib::Request& request, httplib::Response& response) {
    std::string id = request.path_params.at("id");

    if (clients.find(id) == clients.end()) {
        response.status = 403;
        response.set_content("Resource forbidden", 10, "text/plain");
        return;
    }

    auto current_time = std::chrono::system_clock::now();
    auto end_time = current_time + 1h;
    DES des_encryptor;
    std::string TGT = id + ";" + TGS + ";" +
                      std::to_string(std::chrono::system_clock::to_time_t(current_time)) + ";" +
                      std::to_string(std::chrono::system_clock::to_time_t(end_time)) + ";" + clients[id].key;
    des_encryptor.setKey(PASSWORD);

    std::string TGT_encrypted = des_encryptor.encryptAnyString(TGT);

    std::string answer = TGT_encrypted + ";" + clients[id].tgs;

    des_encryptor.setKey(clients[id].key);

    std::cout << "Encrypting message: '" << answer << "' to '" << id << "'" << std::endl;

    string encrypted = des_encryptor.encryptAnyString(answer);

    // cout << "Sending message: '" << encrypted << "' to '" << id << "'" << endl;

    response.set_content(encrypted.c_str(), encrypted.size(), "text/plain");
    response.status = 200;
}


void grant(const httplib::Request& request, httplib::Response& response) {
    string id = request.path_params.at("id");

    if (clients.find(id) == clients.end()) {
        response.status = 403;
        response.set_content("Forbidden", 10, "text/plain");
        return;
    }

    auto data = Utils::Split(request.body, ';');

    cout << "Received data in grant from client '" << id << "':" << endl;

    for (const auto& cur : data) {
        cout << cur << endl;
    }

    DES c;
    c.setKey(clients[id].tgs_key);

    auto aut = Utils::Split(c.decryptAnyString(data[1]), ';');

    cout << "Received aut from client '" << id << "':" << endl;

    for (const auto& cur : aut) {
        cout << cur << endl;
    }

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

    auto tgt = Utils::Split(c.decryptAnyString(data.front()), ';');

    cout << "Received tgt from client '" << id << "':" << endl;

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

    cout << "Encrypting response to '" << id << "': " << answer << endl;

    c.setKey(clients[id].tgs_key);

    string answer_encrypted = c.encryptAnyString(answer);

    response.body = answer_encrypted;
    response.status = 200;
}

int main() {
        httplib::Server svr;
    svr.Get("/init:id", initMessage);

    svr.listen("0.0.0.0", 8080);
}
