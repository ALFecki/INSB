#include "conn.h"

Connection::Connection(boost::asio::io_context& io_context) : sock(io_context) {
	buffer.prepare(1024);
}

std::shared_ptr<Connection> Connection::create(boost::asio::io_context& io_context) {
	return std::shared_ptr<Connection>(new Connection(io_context));
}

boost::asio::ip::tcp::socket& Connection::socket() {
	return sock;
}

void Connection::printMessage(const std::string& msg) {
	std::cout << msg << std::endl;
}