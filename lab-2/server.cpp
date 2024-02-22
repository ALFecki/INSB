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

    std::string TGT = id + ";" + TGS + ";" +
                      std::to_string(std::chrono::system_clock::to_time_t(current_time)) + ";" +
                      std::to_string(std::chrono::system_clock::to_time_t(end_time)) + ";" + clients[id].key;
}

int main() {
    httplib::Server svr;

    svr.Get("/hi", [](const httplib::Request& req, httplib::Response& res) {
        res.set_content("Hello World!", "text/plain");
    });

    svr.listen("0.0.0.0", 8080);
}
