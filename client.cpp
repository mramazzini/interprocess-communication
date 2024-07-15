#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <pthread.h>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "message.h"
using namespace std;

// Struct to hold the data for each line of the input file

int main(int argc, char *argv[])
{
    // Usage ./exec_filename hostname port_no < input_filename
    if (argc != 3)
    {
        cerr << "Usage: " << argv[0] << " hostname port_no" << endl;
        return 1;
    }

    string hostname = argv[1];
    int PORT = atoi(argv[2]);

    // Create the arguments list
    static vector<string> list;

    // Read lines from standard input
    string line;
    while (getline(cin, line))
    {
        list.push_back(line);
    }

    // create socket

    // Create a socket
    int sock = 0;
    struct sockaddr_in serv_addr;

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

    // send the number of messages
    string numMessages = to_string(list.size());
    send(sock, numMessages.c_str(), numMessages.length(), 0);

    // Create the messages

    for (int i = 0; i < list.size(); i++)
    {
        string message = list[i];
        // Sending message to the server
        send(sock, message.c_str(), message.length(), 0);

        // Clearing the buffer
        memset(buffer, 0, 1024);
        sleep(0.5);
    }

    // read the responses
    for (int i = 0; i < list.size(); i++)
    {
        int valread = read(sock, buffer, 1024);
        std::cout << buffer << std::endl;
        memset(buffer, 0, 1024);
    }

    // Closing the socket
    close(sock);

    return 0;
}