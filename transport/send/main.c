#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdbool.h>

#define PROTOCOL_ID 253

int main(int argc, char **argv) {

    if (argc != 2) {
        printf("Usage: %s <dest-ip>\n", argv[0]);
        return 0;
    }

    sa_family_t addrFamily = AF_INET;

    int raw_socket = socket(AF_INET, SOCK_RAW, PROTOCOL_ID);
    if (raw_socket == -1) {
        perror("Error opening raw socket");
        return 1;
    }

    unsigned char testData[] = {1, 2, 3, 4, 5, 6, 7, 8};

    struct sockaddr destAddr;
    switch (inet_pton(addrFamily, argv[1], &destAddr)) {
        case 0:
            printf("Address not valid.\n");
            return 1;

        case -1:
            perror("Error parsing address");
            return 1;
        default:
            break;
    }

    destAddr.sa_family = addrFamily;

    while (true) {
        ssize_t nbytessent = sendto(raw_socket, &testData, sizeof(testData), 0, &destAddr, sizeof(destAddr));
        if (nbytessent == -1) {
            perror("Error sending data");
            return 1;
        }

        printf("Wrote %ld bytes to socket\n", nbytessent);
        sleep(1);
    }
}