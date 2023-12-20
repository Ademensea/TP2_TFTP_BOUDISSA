#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUF_SIZE_DATA 516 // header(4B) + data(512B max)

int main(int argc, char *argv[]) {

    char host[20], file[20];
    strcpy(host, argv[1]);
    strcpy(file, argv[2]);

    // Display host and file information
    write(STDOUT_FILENO, "Host: ", 6);
    write(STDOUT_FILENO, host, strlen(host));
    write(STDOUT_FILENO, "\n", 1);

    write(STDOUT_FILENO, "File: ", 6);
    write(STDOUT_FILENO, file, strlen(file));
    write(STDOUT_FILENO, "\n", 1);

    int n_bytes;
    char file_content[BUF_SIZE_DATA - 4];
    struct addrinfo hints;
    struct addrinfo *res;
    memset(&hints, 0, sizeof(struct addrinfo)); // Initialize to 0
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_family = AF_INET;
    hints.ai_protocol = IPPROTO_UDP;

    int en = getaddrinfo(host, "1069", &hints, &res);
    if (en == -1) {
        perror("getaddrinfo");
        exit(EXIT_FAILURE);
    }

    int sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Create WRQ (Write Request) packet
    char buf[BUF_SIZE_DATA];
    memset(buf, 0, BUF_SIZE_DATA);
    buf[1] = 2; // write mode
    strcpy(buf + 2, argv[2]);
    size_t padding = strlen(buf + 2);
    strcpy(buf + 2 + padding + 1, "octet");
    size_t padding2 = strlen(buf + 3 + padding);

    if ((n_bytes = sendto(sock, buf, padding + padding2 + 4, 0, res->ai_addr, res->ai_addrlen)) == -1) {
        perror("send WRQ");
        exit(EXIT_FAILURE);
    }
    write(STDOUT_FILENO, "WRQ sent: OK\n", 13);

    // Open and read the specified file
    int fd = open(file, O_RDONLY);
    if (fd == -1) {
        perror("Open file");
        exit(EXIT_FAILURE);
    }

    if ((n_bytes = read(fd, file_content, BUF_SIZE_DATA - 4)) == -1) {
        perror("read file");
        exit(EXIT_FAILURE);
    }

    // Create DATA packet
    memset(buf, 0, BUF_SIZE_DATA);
    buf[1] = 3;
    buf[3] = 0;
    strcpy(buf + 4, file_content);
    padding = strlen(buf + 4);

    // Send DATA packet
    if (sendto(sock, buf, padding + 4, 0, res->ai_addr, res->ai_addrlen) == -1) {
        perror("Send DATA");
        exit(EXIT_FAILURE);
    }

    write(STDOUT_FILENO, "DATA packet sent: ", 18);
    for (int i = 0; i < 4; i++) {
        write(STDOUT_FILENO, &buf[i], 1);
    }
    for (int i = 4; i < padding + 4; i++) {
        write(STDOUT_FILENO, &buf[i], 1);
    }
    write(STDOUT_FILENO, "\n", 1);

    // Receive ACK packet
    if (recvfrom(sock, buf, 4, 0, res->ai_addr, &res->ai_addrlen) == -1) {
        perror("Receive ACK");
        exit(EXIT_FAILURE);
    }

    write(STDOUT_FILENO, "Server ACK: ", 12);
    for (int i = 0; i < 4; i++) {
        write(STDOUT_FILENO, &buf[i], 1);
    }
    write(STDOUT_FILENO, "\n", 1);

    exit(EXIT_SUCCESS);
}

