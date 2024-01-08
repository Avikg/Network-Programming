// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define BUFFER_SIZE 1024

// Structure for client data
typedef struct {
    int sock;
    struct sockaddr_in client_addr;
} client_data_t;

// Mutex for file writing
pthread_mutex_t file_mutex = PTHREAD_MUTEX_INITIALIZER;

// Function to handle each connected client in a separate thread
void *handle_client(void *arg) {
    client_data_t client_data = *(client_data_t *)arg;
    char buffer[BUFFER_SIZE];
    int bytes_received;
    FILE *log_file;

    // Receive data from the client
    bytes_received = recv(client_data.sock, buffer, BUFFER_SIZE, 0);
    if (bytes_received < 0) {
        perror("recv");
        close(client_data.sock);
        return NULL;
    }

    // Send acknowledgment back to the client
    send(client_data.sock, buffer, bytes_received, 0);

    // Log the request
    pthread_mutex_lock(&file_mutex);
    log_file = fopen("ping_log.txt", "a");
    if (log_file) {
        fprintf(log_file, "Received ping from %s:%d with RTT %s\n",
                inet_ntoa(client_data.client_addr.sin_addr),
                ntohs(client_data.client_addr.sin_port),
                buffer);
        fclose(log_file);
    }
    pthread_mutex_unlock(&file_mutex);

    // Close the connection and free the allocated memory
    close(client_data.sock);
    free(arg);
    return NULL;
}

int main(int argc, char *argv[]) {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Create a socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);  // Default port

    // Bind the socket to the specified address and port
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_sock, 5) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port 8080...\n");

    // Main server loop to accept and handle incoming connections
    while (1) {
        // Accept a new client connection
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_sock < 0) {
            perror("accept");
            continue;
        }

        // Allocate memory for client data and create a new thread to handle the client
        client_data_t *client_data = malloc(sizeof(client_data_t));
        client_data->sock = client_sock;
        client_data->client_addr = client_addr;

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, client_data);
        pthread_detach(thread_id);
    }

    // Close the server socket (unreachable in this example)
    close(server_sock);
    return 0;
}
