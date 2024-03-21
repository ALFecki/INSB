#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/socket.h>

#include <iostream>
#include <memory>

#define LISTEN_BACKLOG 50

#define handle_error_en(err, msg) \
	do {                            \
		errno = err;                  \
		perror(msg);                  \
		exit(EXIT_FAILURE);           \
	} while (0)

struct FakeHeader {
	unsigned int sourceAddress;
	unsigned int destAddress;
	unsigned char placeholder;
	unsigned char protocol;
	unsigned short tcpLength;

	struct tcphdr tcp;
};

unsigned short checksum(unsigned short *ptr, int nbytes) {
	long sum;
	unsigned short oddbyte;
	short answer;

	sum = 0;
	while (nbytes > 1) {
		sum += *ptr++;
		nbytes -= 2;
	}
	if (nbytes == 1) {
		oddbyte = 0;
		*((u_char *)&oddbyte) = *(u_char *)ptr;
		sum += oddbyte;
	}

	sum = (sum >> 16) + (sum & 0xffff);
	sum = sum + (sum >> 16);
	answer = (short)~sum;

	return (answer);
}

std::unique_ptr<iphdr> fillIPHeader(char datagram[4096], const char source_ip[32], struct sockaddr_in sin) {
	iphdr *ipHeader = (struct iphdr *)datagram;

	ipHeader->ihl = 5;
	ipHeader->version = 4;
	ipHeader->tos = 0;
	ipHeader->tot_len = sizeof(struct ip) + sizeof(struct tcphdr);
	ipHeader->id = htons(54321);
	ipHeader->frag_off = 0;
	ipHeader->ttl = 255;
	ipHeader->protocol = IPPROTO_TCP;
	ipHeader->check = 0;
	ipHeader->saddr = inet_addr(source_ip);
	ipHeader->daddr = sin.sin_addr.s_addr;
	ipHeader->check = checksum((unsigned short *)datagram, ipHeader->tot_len >> 1);

	return std::unique_ptr<iphdr>(ipHeader);
}

std::unique_ptr<tcphdr> fillTCPHeader(char datagram[4096]) {
	struct tcphdr *tcpHeader = (struct tcphdr *)(datagram + sizeof(struct ip));

	tcpHeader->source = htons(1234);
	tcpHeader->dest = htons(8000);
	tcpHeader->seq = 0;
	tcpHeader->ack_seq = 0;
	tcpHeader->doff = 5;
	tcpHeader->fin = 0;
	tcpHeader->syn = 1;
	tcpHeader->rst = 0;
	tcpHeader->psh = 0;
	// tcph->ack = 0;
	tcpHeader->urg = 0;
	tcpHeader->window = htons(5840);
	tcpHeader->check = 0;
	tcpHeader->urg_ptr = 0;

	return std::unique_ptr<tcphdr>(tcpHeader);
}

FakeHeader fillFakeHeader(const char source_ip[32], struct sockaddr_in sin) {
	struct FakeHeader fakeHeader;
	fakeHeader.sourceAddress = inet_addr(source_ip);
	fakeHeader.destAddress = sin.sin_addr.s_addr;
	fakeHeader.placeholder = 0;
	fakeHeader.protocol = IPPROTO_TCP;
	fakeHeader.tcpLength = htons(20);
	return fakeHeader;
}
