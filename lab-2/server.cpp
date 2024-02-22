#include "httplib.h"

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

}


int main() {

    Server svr;

    svr.Get("/hi", [](const Request& req, Response& res) { res.set_content("Hello World!", "text/plain"); });

    svr.listen("0.0.0.0", 8080);
}
