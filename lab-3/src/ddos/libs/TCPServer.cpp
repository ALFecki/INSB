#include "TCPServer.h"

TCPServer::TCPServer(boost::asio::io_context& io_context, short port)
		: io_context_(io_context),
			acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {
	accept();
}

void TCPServer::accept() {
	auto tcp_conn = Connection::create(io_context_);
	acceptor_.async_accept(tcp_conn->socket(),
												 std::bind(&TCPServer::handle_accept, this, tcp_conn, std::placeholders::_1));
}

void TCPServer::handle_accept(std::shared_ptr<Connection> tcp_conn, const boost::system::error_code& ec) {
	if (ec) {
		std::cout << ec.message() << '\n';
	}
	accept();
}