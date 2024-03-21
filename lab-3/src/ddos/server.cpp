#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <iostream>

#include "libs/TCPServer.h"

using namespace boost::asio;
using namespace boost::asio::ip;

using namespace boost::asio;

int main(int argc, char* argv[]) {
	if (argc < 3) {
		std::cerr << "Usage: ./server -p <port>\n";
		return 1;
	}

	if (std::string(argv[1]) != "-p") {
		std::cout << "Unknown flag: " << argv[1] << "\n";
		return 1;
	}

	unsigned short port = static_cast<unsigned short>(std::atoi(argv[2]));

	boost::asio::io_context io_context;
	boost::asio::ip::tcp::acceptor acceptor_(io_context,
																					 boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));

	TCPServer s(io_context, port);
	io_context.run();
	return 0;
}