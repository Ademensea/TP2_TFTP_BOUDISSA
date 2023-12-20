#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#define BUF_SIZE_DATA 516 // en-tete(4o) + data(512o max)

int main(int argc, char *argv[]) {

    // Initialize character arrays to store host and file information
    char host[20], file[20];
    // Copy command line arguments to the host and file arrays
    strcpy(host, argv[1]);
    strcpy(file, argv[2]);
    // Display host information on the standard output
    write(STDOUT_FILENO, "Host : ", 7);
    write(STDOUT_FILENO, host, strlen(host));
    write(STDOUT_FILENO, "\n", 1);

    // Display file information on the standard output
    write(STDOUT_FILENO, "File : ", 7);
    write(STDOUT_FILENO, file, strlen(file));
    write(STDOUT_FILENO, "\n", 1);

    // Declare variables for address information
    int n_bytes;
    struct addrinfo hints;
    struct addrinfo *res;

    // Initialize the hints structure to zero
    memset(&hints, 0, sizeof(struct addrinfo));

    // Specify the socket type, address family, and protocol for address information
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_family = AF_INET;
    hints.ai_protocol = IPPROTO_UDP;

    // Call getaddrinfo to obtain address information for the specified host and port
    int en = getaddrinfo(host, "1069", &hints, &res);

    // Check for errors in getaddrinfo
    if (en == -1) {

        perror("getaddrinfo");
        exit(EXIT_FAILURE);

    }
//Create a socket using the obtained address information
    int sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

    //Check if the socket creation was successful
    if (sock == -1) {

        perror("socket");
        exit(EXIT_FAILURE);
    }

       //Creation request RPQ

    char buf[BUF_SIZE_DATA];
    memset(buf, 0, BUF_SIZE_DATA);
    buf[1] = 1;
    strcpy(buf + 2, argv[2]);
    size_t padding = strlen(buf+2);
    strcpy(buf + 2 + padding + 1, "octet");
    size_t padding2 = strlen(buf + 3 + padding);


        if (sendto(sock, buf, padding + padding2 + 4, 0, res->ai_addr, res->ai_addrlen) == -1) {

            perror("send RRQ");

            exit(EXIT_FAILURE);

        }

    n_bytes = BUF_SIZE_DATA ;

    while (n_bytes == BUF_SIZE_DATA) {
        // First reception
        if ((n_bytes = recvfrom(sock, buf, BUF_SIZE_DATA, 0, res->ai_addr, &res->ai_addrlen)) == -1) {
            perror("receive DATA");
            exit(EXIT_FAILURE);
        }


        write(STDOUT_FILENO, "En-tête : ", 10);
        for (int i = 0; i < 4; i++) {
            dprintf(STDOUT_FILENO, "%x", buf[i]);
        }
        write(STDOUT_FILENO, "\nContenu du fichier : ", 22);
        for (int i = 4; i < n_bytes; i++) {
            dprintf(STDOUT_FILENO, "%d", buf[i]);
        }
        write(STDOUT_FILENO, "\n", 1);

        //Send ACK
        buf[1] = 4;
        write(STDOUT_FILENO, "Envoie ACK : ", 14);
        for (int i = 0; i < 4; i++) {
            dprintf(STDOUT_FILENO, "%d", buf[i]);
        }
        write(STDOUT_FILENO, "\n", 1);
    }

// Send ACK to server

    if (sendto(sock, buf, 4, 0, res->ai_addr, res->ai_addrlen) == -1) {
        perror("Send ACK");
        exit(EXIT_FAILURE);
    }
    write(STDOUT_FILENO, "ACK send\n", 9);

// Close the socket
    close(sock);
    freeaddrinfo(res);

// Finish
    exit(EXIT_SUCCESS);




}