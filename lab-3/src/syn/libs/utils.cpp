// #include <arpa/inet.h>
// #include <errno.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <memory>
// #include <pthread.h>
// #include <stdbool.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
#include <sys/socket.h>
// #include <sys/types.h>
// #include <unistd.h>

// #include <atomic>
#include <iostream>

#define handle_error_en(err, msg) \
	do {                            \
		errno = err;                  \
		perror(msg);                  \
		exit(EXIT_FAILURE);           \
	} while (0)

struct FakeHeader	 // for checksum
{
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
	iphdr *iph = (struct iphdr *)datagram;

	iph->ihl = 5;
	iph->version = 4;
	iph->tos = 0;
	iph->tot_len = sizeof(struct ip) + sizeof(struct tcphdr);
	iph->id = htons(54321);
	iph->frag_off = 0;
	iph->ttl = 255;
	iph->protocol = IPPROTO_TCP;
	iph->check = 0;
	iph->saddr = inet_addr(source_ip);
	iph->daddr = sin.sin_addr.s_addr;
	iph->check = checksum((unsigned short *)datagram, iph->tot_len >> 1);

    return std::unique_ptr<iphdr>(iph);
}

std::unique_ptr<tcphdr> fillTCPHeader(char datagram[4096] ) {
    struct tcphdr *tcph = (struct tcphdr *)(datagram + sizeof(struct ip));

    tcph->source = htons(1234);
    tcph->dest = htons(8000);
    tcph->seq = 0;
    tcph->ack_seq = 0;
    tcph->doff = 5;
    tcph->fin = 0;
    tcph->syn = 1;
    tcph->rst = 0;
    tcph->psh = 0;
    // tcph->ack = 0;
    tcph->urg = 0;
    tcph->window = htons(5840);
    tcph->check = 0;
    tcph->urg_ptr = 0;

    return std::unique_ptr<tcphdr>(tcph);
}

