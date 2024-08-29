#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// #include <arpa/inet.h>
// https://stackoverflow.com/questions/23730455/cannot-open-include-file-arpa-inet-h
#include <Winsock2.h>
#include <sys/types.h>
// #include <sys/socket.h>
// https : // stackoverflow.com/questions/6769981/windows-socket-programming-in-c
// https://learn.microsoft.com/en-gb/windows/win32/api/winsock/nf-winsock-wsastartup
#include <WinSock.h>

#define PORT 12345
#define BUFFER_SIZE 1024

void handle_client(int client_socket)
{
    char buffer[BUFFER_SIZE];
    int bytes_read;

    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE);

        bytes_read = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_read <= 0)
        {
            perror("recv");
            close(client_socket);
            return;
        }

        printf("Client: %s", buffer);

        // Send response to the client
        printf("Server: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        send(client_socket, buffer, strlen(buffer), 0);
    }
}

int main()
{
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;

    // Create socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket to address
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) == -1)
    {
        perror("listen");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    addr_size = sizeof(client_addr);

    // Accept incoming connection
    if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size)) == -1)
    {
        perror("accept");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Client connected.\n");

    // Handle client communication
    handle_client(client_socket);

    close(server_socket);
    return 0;
}
