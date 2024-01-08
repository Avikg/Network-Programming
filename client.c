// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int client_sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    int port = 8080;  // Default port
    char *server_ip = "127.0.0.1";  // Default IP
    int num_requests = 4;  // Default number of requests
    int interval = 1;  // Default interval in seconds

    // Parse command line arguments
    if (argc > 1) {
        server_ip = argv[1];
    }
    if (argc > 2) {
        num_requests = atoi(argv[2]);
    }
    if (argc > 3) {
        interval = atoi(argv[3]);
    }
    if (argc > 4) {
        port = atoi(argv[4]);
    }

    // Loop for making multiple requests
    for (int i = 0; i < num_requests; i++) {
        // Create a socket
        client_sock = socket(AF_INET, SOCK_STREAM, 0);
        if (client_sock < 0) {
            perror("socket");
            exit(EXIT_FAILURE);
        }

        // Configure server address structure
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr(server_ip);
        server_addr.sin_port = htons(port);

        // Connect to the server
        if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
            perror("connect");
            close(client_sock);
            continue;  // Continue to the next iteration in case of connection failure
        }

        // Generate timestamp as a string and send it to the server
        snprintf(buffer, BUFFER_SIZE, "%ld", time(NULL));
        send(client_sock, buffer, strlen(buffer), 0);

        // Receive acknowledgment from the server
        recv(client_sock, buffer, BUFFER_SIZE, 0);
        printf("Received acknowledgment: %s with RTT: %ld ms\n", buffer, (time(NULL) - atol(buffer)) * 1000);

        // Close the socket and wait for the specified interval
        close(client_sock);
        sleep(interval);
    }

    return 0;
}
