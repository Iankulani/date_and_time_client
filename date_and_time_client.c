#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1024

void start_client() {
    char server_host[50];
    int server_port;
    
    // Prompt for the server's IP address and port
    printf("Enter the server's IP address:");
    scanf("%s", server_host);
    printf("Enter the server's port number:");
    scanf("%d", &server_port);
    
    // Create a socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        exit(1);
    }
    
    // Set up the server address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    
    // Convert the IP address from text to binary form
    if (inet_pton(AF_INET, server_host, &server_addr.sin_addr) <= 0) {
        perror("Invalid server address");
        close(client_socket);
        exit(1);
    }
    
    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        close(client_socket);
        exit(1);
    }
    
    // Receive the date and time from the server
    char date_time[MAX_BUFFER_SIZE];
    int bytes_received = recv(client_socket, date_time, sizeof(date_time) - 1, 0);
    if (bytes_received < 0) {
        perror("Error receiving data from server");
    } else {
        date_time[bytes_received] = '\0';  // Null-terminate the received string
        printf("Received from server: %s\n", date_time);
    }
    
    // Close the socket
    close(client_socket);
}

int main() {
    start_client();
    return 0;
}
