#include "httplib.h"

int main() {
  using namespace httplib;

  Server svr;

  svr.Get("/hi", [](const Request& req, Response& res) {
    res.set_content("Hello World!", "text/plain");
  });

  svr.listen("0.0.0.0", 8081);
}

