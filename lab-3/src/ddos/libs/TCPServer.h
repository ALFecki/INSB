#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <iostream>

#include "Connection.h"

class TCPServer {
public:
	TCPServer(boost::asio::io_context&, short);

private:
	void accept();

	void handle_accept(std::shared_ptr<Connection>, const boost::system::error_code&);
	void handle_read(std::shared_ptr<Connection> tcp_conn, const boost::system::error_code& ec, std::size_t bytes_transferred);

	boost::asio::ip::tcp::acceptor acceptor_;
	boost::asio::io_context& io_context_;
};