#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <time.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/ip.h>
#include<sys/socket.h>
#include<arpa/inet.h>


#define PROTOCOL_ID 253
#define BUF_SIZE 1024

int main(int argc, char **argv) {
    
    sa_family_t addrFamily = AF_INET;

    int sock = socket(addrFamily, SOCK_RAW, PROTOCOL_ID);
    if (sock == -1) {
        perror("Error opening raw socket");
        return 1;
    }

    ssize_t recvCount = 0;
    char buffer[BUF_SIZE];

    while ((recvCount = recv(sock, &buffer, sizeof(buffer), 0)) != -1) {

        struct iphdr *ip_packet = (struct iphdr *) buffer;
        struct sockaddr_in source_socket_address = {0};
        source_socket_address.sin_addr.s_addr = ip_packet->saddr;

        printf("Source Address: %s\n", inet_ntoa(source_socket_address.sin_addr));
        printf("%ld bytes: %.*s\n", recvCount, (int) recvCount - ip_packet->ihl * 4, buffer + ip_packet->ihl * 4);
    }
    perror("Error receiving");
}