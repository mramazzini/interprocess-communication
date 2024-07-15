
// IGNORE THIS FILE THIS IS JUST FOR ME TRYING TO FIGURE OUT HOW TO MAKE THIS WORK

#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 3001

int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    const char *message = "Hello from client";
    char buffer[1024] = {0};

    // Creating socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }

    // Connecting to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cerr << "Connection Failed" << std::endl;
        return -1;
    }

    while (true)
    {
        std::string message;
        std::cout << "Enter message to send (or type 'exit' to quit): ";
        std::getline(std::cin, message);
        if (message == "exit")
        {
            break;
        }

        // Sending message to the server
        send(sock, message.c_str(), message.length(), 0);
        std::cout << "Message sent: " << message << std::endl;

        // Reading server's response
        int valread = read(sock, buffer, 1024);
        std::cout << "Server response: " << buffer << std::endl;

        // Clearing the buffer
        memset(buffer, 0, 1024);
    }

    // Closing the socket
    close(sock);
    return 0;
}