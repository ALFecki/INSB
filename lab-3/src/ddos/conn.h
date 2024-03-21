#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <iostream>

using namespace boost::asio;
using namespace boost::asio::ip;

using namespace boost::asio;

class Connection : public std::enable_shared_from_this<Connection> {
public:
	ip::tcp::socket& socket();

	static std::shared_ptr<Connection> create(boost::asio::io_context&);

private:
	Connection(boost::asio::io_context&);

	void printMessage(const std::string&);

	ip::tcp::socket sock;
	streambuf buffer;
};