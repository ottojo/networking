#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <time.h>
#include <memory.h>

#define PROTOCOL_ID 253

int main(int argc, char **argv) {

    if (argc != 2) {
        printf("Usage: %s <dest-ip>\n", argv[0]);
        return 0;
    }

    // Use IPv4
    sa_family_t addrFamily = AF_INET;

    // Open a raw socket with our own protocol number
    int raw_socket = socket(AF_INET, SOCK_RAW, PROTOCOL_ID);
    if (raw_socket == -1) {
        perror("Error opening raw socket");
        return 1;
    }

    struct sockaddr_in destinationSocketAddr = {0};
    destinationSocketAddr.sin_family = addrFamily;

    // Parse the destination IP from the command line parameter
    switch (inet_pton(addrFamily, argv[1], &destinationSocketAddr.sin_addr)) {
        case 0:
            printf("Address not valid.\n");
            return 1;

        case -1:
            perror("Error parsing address");
            return 1;
        default:
            break;
    }


    while (true) {
        // Send hello world

        char *message = "Hello World";

        ssize_t bytesSent = sendto(raw_socket, message, strlen(message), 0,
                                   (const struct sockaddr *) &destinationSocketAddr, sizeof(destinationSocketAddr));
        if (bytesSent == -1) {
            perror("Error sending data");
            return 1;
        }

        printf("Sent %ld bytes to %s\n", bytesSent, inet_ntoa(destinationSocketAddr.sin_addr));
        sleep(1);
    }
}