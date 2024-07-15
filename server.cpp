
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "message.h"
#include <cstring>
using namespace std;

// Recursive XOR binary division algorithm that returns the remainder
struct XORDivisionArgs
{
    BinaryInt dividend;
    BinaryInt divisor;
    BinaryInt *finalRemainder;
    XORDivisionArgs() : dividend(), divisor()
    {
        finalRemainder = new BinaryInt;
    }
};

void *XORDivision(void *ptr)
{
    XORDivisionArgs *args = (XORDivisionArgs *)ptr;
    BinaryInt dividend = args->dividend;
    BinaryInt divisor = args->divisor;
    BinaryInt *finalRemainder = args->finalRemainder;
    vector<int> result;
    if (dividend.binary.size() < divisor.binary.size())
    {

        *finalRemainder = dividend;
        return NULL;
    }

    for (int i = 0; i < divisor.binary.size(); i++)
    {
        if (dividend.binary[i] == divisor.binary[i])
        {
            result.push_back(0);
        }
        else
        {
            result.push_back(1);
        }
    }

    // add the remaining bits of the dividend
    for (int i = divisor.binary.size(); i < dividend.binary.size(); i++)
    {
        result.push_back(dividend.binary[i]);
    }

    // clear leading zeros
    while (!result.empty() && result[0] == 0)
    {
        result.erase(result.begin());
    }

    BinaryInt remainder(result);

    // XORDivisionArgs *newArgs = new XORDivisionArgs;
    // newArgs->dividend = remainder;
    // newArgs->divisor = divisor;
    // newArgs->finalRemainder = finalRemainder;
    // cout << "Remainder: ";
    // remainder.print();

    XORDivisionArgs newArgs;
    newArgs.dividend = remainder;
    newArgs.divisor = divisor;
    newArgs.finalRemainder = finalRemainder;

    return XORDivision((void *)&newArgs);
};

// Function that generates the encoded message. This function is called by the threads in main. It also generates new threads to perform the XOR division
void *calcRemainder(void *ptr)
{
    // cast the void pointer to the struct and extract the data
    message *arguments = (message *)ptr;
    BinaryInt *key = arguments->key;
    vector<BinaryInt *> inputsBinary = arguments->inputsBinary;
    args *ref = arguments->ref;
    int order = arguments->order;
    vector<BinaryInt *> remainders = ref->remainders;

    // create new binary ints with the zeros appended
    vector<BinaryInt *> newInputs;

    for (int i = 0; i < inputsBinary.size(); i++)
    {
        vector<int> newBinary = inputsBinary[i]->binary;
        for (int j = 0; j < order; j++)
        {
            newBinary.push_back(0);
        }
        newInputs.push_back(new BinaryInt(newBinary));
    }

    // threads
    pthread_t threads[newInputs.size()];
    vector<XORDivisionArgs *> args;

    // XOR division the new inputs and replace zeros with the remainder
    for (int i = 0; i < newInputs.size(); i++)
    {
        XORDivisionArgs *newArgs = new XORDivisionArgs;
        newArgs->dividend = *newInputs[i];
        newArgs->divisor = *key;

        args.push_back(newArgs);
        pthread_create(&threads[i], NULL, XORDivision, (void *)newArgs);
    }

    for (int i = 0; i < newInputs.size(); i++)
    {
        pthread_join(threads[i], NULL);
    }

    // add leading zeros to the remainder so that length = order
    for (int i = 0; i < newInputs.size(); i++)
    {
        vector<int> remainder = args[i]->finalRemainder->binary;
        while (remainder.size() < order)
        {
            remainder.insert(remainder.begin(), 0);
        }
        remainders.push_back(new BinaryInt(remainder));
    }

    // attach the messages to the reference
    ref->messages = newInputs;
    ref->remainders = remainders;

    return NULL;
};

string getMessageString(args *msg)
{
    string response = "Key: ";

    response += msg->key.getBinaryString();
    response += ", CRC values: ";

    for (int k = 0; k < msg->remainders.size(); k++)
    {
        response += to_string(msg->inputs[k]);
        response += " = ";
        response += msg->inputsBinary[k]->getBinaryString();
        response += msg->remainders[k]->getBinaryString();
        if (k != msg->remainders.size() - 1)
        {
            response += ", ";
        }
    }

    return response;
}

int main(int argc, char *argv[])
{
    // Server code
    // Usage ./exec_filename port_no
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " port_no" << endl;
        return 1;
    }

    int PORT = atoi(argv[1]);
    const int BUFFER_SIZE = 1024;

    // Need to send the messages back to the client
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Binding the socket to the port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listening for incoming connections
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Accepting an incoming connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // read the number of messages
    int numMessages;
    read(new_socket, &buffer, sizeof(buffer));
    numMessages = stoi(buffer);

    // create threads for each message
    pthread_t threads[numMessages];
    message messageList[numMessages];

    static vector<args *> argumentsList;

    for (int i = 0; i < numMessages; i++)
    {
        // Clearing the buffer
        memset(buffer, 0, BUFFER_SIZE);

        // Reading message from the client
        int valread = read(new_socket, buffer, BUFFER_SIZE);
        if (valread <= 0)
        {
            std::cout << "Client disconnected or error reading" << std::endl;
            break;
        }

        argumentsList.push_back(new args(buffer));

        // Create the message struct
        messageList[i].key = &argumentsList[i]->key;
        messageList[i].inputsBinary = argumentsList[i]->inputsBinary;
        messageList[i].order = argumentsList[i]->order;
        messageList[i].ref = argumentsList[i];

        memset(buffer, 0, 1024);
    }

    // create the threads
    for (int i = 0; i < numMessages; i++)
    {
        pthread_create(&threads[i], NULL, calcRemainder, (void *)&messageList[i]);
    }

    // join the threads
    for (int i = 0; i < numMessages; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // send the responses back to the client
    for (int i = 0; i < numMessages; i++)
    {
        string response = getMessageString(argumentsList[i]);
        send(new_socket, response.c_str(), response.length(), 0);
        sleep(1);
    }

    // Closing the socket
    close(new_socket);
    close(server_fd);
    return 0;
};
