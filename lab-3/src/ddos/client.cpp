#include <boost/asio.hpp>
#include <iostream>
#include <stdexcept>

using namespace boost::asio;
using namespace boost::asio::ip;

void handle_connect(const boost::system::error_code& error) {
	if (error) {
		std::cerr << "false" << std::endl;
		throw std::runtime_error("Cannot connect.");
	} else {
		std::cerr << "true" << std::endl;
	}
}

int main(int argc, char* argv[]) {
	try {
		if (argc < 7) {
			std::cerr << "Fatal error: not enough arguments." << std::endl;
			std::cerr << "Usage: ./client -h <host> -p <port> -N <number of connections>" << std::endl;
			return 1;
		}

		std::string host;
		unsigned short port;
		int connectionNum;

		for (int i = 1; i < argc; i++) {
			std::string arg = argv[i];

			if (arg == "-h") {
				if (i + 1 < argc) {
					host = argv[i + 1];
					i++;
				} else {
					std::cerr << "Error: no value is specified for -h." << std::endl;
					return 1;
				}
			} else if (arg == "-p") {
				if (i + 1 < argc) {
					port = static_cast<unsigned short>(std::atoi(argv[i + 1]));
					i++;
				} else {
					std::cerr << "Error: no value is specified for -p." << std::endl;
					return 1;
				}
			} else if (arg == "-N") {
				if (i + 1 < argc) {
					connectionNum = static_cast<int>(std::atoi(argv[i + 1]));
					i++;
				} else {
					std::cerr << "Error: no value is specified for -N." << std::endl;
					return 1;
				}
			}
		}

		boost::asio::io_context io_context;
		tcp::resolver resolver(io_context);
		tcp::resolver::query query(tcp::v4(), host, std::to_string(port));
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
		std::vector<tcp::socket*> sockets;
		std::vector<boost::asio::io_context*> contexts;
		sockets.reserve(connectionNum);
		contexts.reserve(connectionNum);

		for (int cnt = 0; cnt < connectionNum; ++cnt) {
			auto ctx_ptr = new boost::asio::io_context();
			auto skt_ptr = new tcp::socket(*ctx_ptr);
			
            contexts.push_back(ctx_ptr);
			sockets.push_back(skt_ptr);
			
            try {
				sockets.back()->async_connect(*endpoint_iterator, handle_connect);
				contexts.back()->run();
			} catch (...) {
				std::cout << "Exception handled.\n";
				break;
			}
		}
		io_context.run();
		
        for (auto& skt : sockets) {
			skt->close();
			delete skt;
		}
		
        for (auto& ctx : contexts) {
			delete ctx;
		}
		
        std::cout << connectionNum << " connections to " << host << ":" << port << "\n";
	} catch (std::exception& e) {
		std::cerr << "Error: " << e.what() << "\n";
	}

	return 0;
}