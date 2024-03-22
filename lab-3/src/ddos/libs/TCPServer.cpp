#include "TCPServer.h"

TCPServer::TCPServer(boost::asio::io_context& io_context, short port)
		: io_context_(io_context),
			acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {
	accept();
	std::cout << "Server started on port " << port << std::endl;
}

void TCPServer::accept() {
	auto tcp_conn = Connection::create(io_context_);
	acceptor_.async_accept(tcp_conn->socket(),
												 std::bind(&TCPServer::handle_accept, this, tcp_conn, std::placeholders::_1));
}

void TCPServer::handle_accept(std::shared_ptr<Connection> tcp_conn, const boost::system::error_code& ec) {
	if (ec) {
		std::cout << "Error in connecting: " << ec.message() << '\n';
	} else {
		std::cout << "New connection № " << ++this->connectionNum << " established!" << std::endl;
	}
	accept();
}

void TCPServer::handle_read(std::shared_ptr<Connection> tcp_conn,
														const boost::system::error_code& ec,
														std::size_t bytes_transferred) {
	//   if (ec) {
	//     std::cout << "Error during read: " << ec.message() << std::endl;
	//   } else {
	//     // Выводим принятое сообщение в консоль
	//     std::cout << "Received message: " << std::string(tcp_conn->buffer().data(), bytes_transferred) <<
	//     std::endl;

	//     // Продолжаем чтение данных от клиента
	//     tcp_conn->socket().async_read_some(boost::asio::buffer(tcp_conn->buffer()),
	//                                        std::bind(&TCPServer::handle_read, this, tcp_conn,
	//                                        std::placeholders::_1,
	//                                                  std::placeholders::_2));
	//   }
}