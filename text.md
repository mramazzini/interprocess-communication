This programming assignment closes at 11:58:59 PM on 07/07/2024.
 
Similarity Threshold: 90%
 
Objective:
This assignment will introduce you to interprocess communication mechanisms in UNIX using sockets.
 
Problem:
You must write two programs to implement a distributed version of the program to determine the Cyclic Redundancy Check (CRC) codes of multiple positive integer values you created for programming assignment 1. 

These programs are:

The server program:

The user will execute this program using the following syntax:
 
./exec_filename port_no
 
where exec_filename is the name of your executable file and port_no is the port number to create the socket. The port number will be available to the server program as a command-line argument.
 
The server program does not receive any information from STDIN and does not print any messages to STDOUT.
 
The server program executes the following task:

        Receive multiple requests from the client program using sockets. Therefore, the server program creates a child process per request to handle these requests simultaneously. For this reason, the parent process must handle zombie processes by implementing the fireman() function call (unless you can determine the number of requests the server program receives from the client program). 

Each child process executes the following tasks:

        First, receive the input string with the integer values to encode and the generator polynomial (key).
        Next, use the CRC Encoding Algorithm to determine the CRC values of the input values.
        Finally, return the calculated CRC values to the client program using sockets.

 
The client program:
 
The user will execute this program using the following syntax:
 
./exec_filename hostname port_no < input_filename
 
where exec_filename is the name of your executable file, hostname is the address where the server program is located, port_no is the port number used by the server program, and input_filename is the name of the input file. The hostname and the port number will be available to the client as command-line arguments.
The client program receives from STDIN (using input redirection) n lines (where n is the number of input strings). Each line from the input represents the input values to encode and the generator polynomial (key)
 
Example Input:


4 12 5 9 36 1101 
3 7 11 23 10001
1 32 101
5 1 2 3 4 5 1010

  

After reading the information from STDIN, this program creates n child threads (where n is the number of strings from the input). Each child thread executes the following tasks: 

    Receives the string with the input values to incode and the key from the main thread.
    Create a socket to communicate with the server program.
    Send the input values to encode and the key to the server program using sockets. 
    Wait for the CRC values from the server program.
    Write the received information into a memory location accessible by the main thread.

Finally, after receiving the entropy values, the main thread prints the CRC values. Given the previous input, the expected output is:


Key: 1101, CRC values: 12 = 1100101, 5 = 101110, 9 = 1001011, 36 = 100100001
Key: 10001, CRC values: 7 = 1110111, 11 = 10111011, 23 = 101110110
Key: 101, CRC values: 32 = 10000010
Key: 1010, CRC values: 1 = 1010, 2 = 10100, 3 = 11110, 4 = 100010, 5 = 101000


Notes:

        You can safely assume that the input will always be in the proper format.
        You must use the output statement format based on the example above.
        For the client program, you must use POSIX Threads and stream sockets. A penalty of 100% will be applied to submissions not using POSIX Threads and Stream Sockets.
        You must use multiple processes (fork) and stream sockets for the server program. A penalty of 100% will be applied to submissions not using multiple processes and Stream Sockets.
        The Moodle server will kill your server program after it is done executing each test case.
        You must present code that is readable and has comments explaining the logic of your solution. A 10% penalty will be applied to submissions not following this guideline.
        You cannot use global variables. A 100% penalty will be applied to submissions using global variables.
        A penalty of 100% will be applied to solutions that do not compile.
        A penalty of 100% will be applied to solutions hardcoding the output.

RUBRIC:

    10 points for each test case (20 points total)
    10 points for presenting clean and readable code.
    10 points for using the fireman function correctly or using a for loop based on the number of requests.
    10 points for reading information from STDIN (client).
    25 points if each request to the server:
        Receives information from the client program and creates a child process per request (5 points).
        Uses the CRC Encoding Algorithm (10 points).
        Returns the CRC values back to the client program (5 points).
        Closes the socket (5 points).
    25 points if each child thread:
        Creates a socket to communicate with the server (5 points).
        Sends and receives information to/from the server (10 points).
        Stores the information on a memory location accessible by the main thread (5 points).
        Closes the socket (5 points).

ADDITIONAL PENALTIES:
100 points off for a solution not compiling (in addition to test cases not passing)
100 points off for lack of generalized solution (hard coding, no server file, etc.)
100 points off for not using POSIX threads, fork(), or Sockets.
100 points for using global variables.

Requested files
client.cpp

server.cpp

