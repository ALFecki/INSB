#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <iostream>

class Connection : public std::enable_shared_from_this<Connection> {
public:
	boost::asio::ip::tcp::socket& socket();

	static std::shared_ptr<Connection> create(boost::asio::io_context&);

private:
	Connection(boost::asio::io_context&);

	void printMessage(const std::string&);

	boost::asio::ip::tcp::socket sock;
	boost::asio::streambuf buffer;
};