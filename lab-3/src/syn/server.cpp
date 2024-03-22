#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <pthread.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <atomic>
#include <iostream>
#include <memory>
#include <thread>

#include "libs/utils.cpp"

int sock_client;

std::atomic<long long> G_ACK_CNT(0);
std::atomic<long long> G_SYN_CNT(0);

bool is_connected = false;

void *tcp_shk(void *arg) {
	char datagram[4096], source_ip[32];

	strcpy(source_ip, "127.0.0.1");
	memset(datagram, 0, 4096);

	struct sockaddr_in sin;

	sin.sin_family = AF_INET;
	sin.sin_port = htons(8000);
	sin.sin_addr.s_addr = inet_addr(source_ip);

	auto iph = fillIPHeader(datagram, source_ip, sin);
	auto tcph = fillTCPHeader(datagram);
	tcph->ack = 1;

	auto psh = fillFakeHeader(source_ip, sin);

	memcpy(&psh.tcp, tcph, sizeof(struct tcphdr));

	tcph->check = checksum((unsigned short *)&psh, sizeof(FakeHeader));

	send(sock_client, datagram, strlen(datagram), 0);

	G_ACK_CNT++;
	std::cout << "ACK №" << G_ACK_CNT << " sended" << std::endl;
	char msg[4000];

	ssize_t err = recv(-1, msg, 4000, 0);
	pthread_exit(0);
}

void *server_receive(void *arg) {
	char msg[4000];

	while (true) {
		ssize_t err = recv(sock_client, msg, 4000, 0);
		
		if (err > 0) {
			msg[3999] = '\0';

			G_SYN_CNT++;
			std::cout << "Connection SYN №" << G_SYN_CNT << " received" << std::endl;
			
			pthread_t thread;
			pthread_create(&thread, NULL, tcp_shk, NULL);
		}

		if (err == 0) {
			break;
		}

		if (err == -1) {
			handle_error_en(err, "recv");
		}
	}

	pthread_exit(0);
}

int main(int argc, char **argv) {

	pthread_attr_t attr;

	int err = pthread_attr_init(&attr);

	if (err != 0) {
		handle_error_en(err, "pthread_attr_init");
	}

	int socket_descr = socket(AF_INET, SOCK_STREAM, 0);

	if (socket_descr == -1) {
		handle_error_en(socket_descr, "socket(2)");
	}

	struct sockaddr_in server_sa;
	server_sa.sin_family = AF_INET;
	server_sa.sin_port = htons(8000);
	server_sa.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (bind(socket_descr, (struct sockaddr *)&server_sa, sizeof(server_sa)) == -1) {
		handle_error_en(-1, "bind");
		return 1;
	}

	if (listen(socket_descr, LISTEN_BACKLOG) == -1) {
		handle_error_en(-1, "listen");
		return 1;
	}

	int client_count = 0;

	std::string server_addr = inet_ntoa(server_sa.sin_addr);
	in_port_t server_port = server_sa.sin_port;

	std::cout << "Server started." << std::endl;
	std::cout << "Host: " << server_addr << ":" << server_port << std::endl;

	while (true) {
		struct sockaddr_in client_sa;

		socklen_t peer_addr_size = sizeof(struct sockaddr_in);
		int sck = accept(socket_descr, (struct sockaddr *)&client_sa, (socklen_t *)&peer_addr_size);

		std::string client_addr = inet_ntoa(client_sa.sin_addr);
		in_port_t client_port = client_sa.sin_port;

		if (!is_connected && sck != -1) {
			sock_client = sck;
			std::cout << "New client connection: " << client_addr << ":" << client_port << std::endl;

			pthread_t thread;
			pthread_create(&thread, &attr, server_receive, NULL);

			is_connected = true;
			std::cout << "Client count: " << ++client_count << std::endl;

			fflush(stdout);
		}
	}

	err = pthread_attr_destroy(&attr);

	if (err != 0)
		handle_error_en(err, "pthread_attr_destroy");

	if (close(socket_descr) == -1)
		handle_error_en(-1, "close");

	return 0;
}