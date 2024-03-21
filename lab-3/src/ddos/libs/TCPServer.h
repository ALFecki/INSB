#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <iostream>

#include "libs/Connection.h"

class TCPServer {
public:
	TCPServer(boost::asio::io_context&, short);

private:
	void accept();

	void handle_accept(std::shared_ptr<Connection>, const boost::system::error_code&);

	boost::asio::ip::tcp::acceptor acceptor_;
	boost::asio::io_context& io_context_;
};