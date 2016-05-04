/*
    A lot of the initial socket code was influenced by the examples placed by Beej
    in the Network Programming Guide; we found his section on GetAddrInfo especially
    helpful when developing an initial client-server model.
*/


// C++ headers
#include <fstream>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <thread>
#include <csignal>

// Kernel Includes
#include <sys/socket.h>
#include <sys/types.h> 
#include <netinet/in.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <netdb.h>
#include <unistd.h>

// User-Defined module includes
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "Helper.h"

using namespace std;

typedef struct addrinfo AddressInfo;
typedef struct sockaddr_in SocketAddress;

//const int BUFFER_LEN = 20;

// these have to be global for signal handler to use them
int socketDsc;                              // Socket descriptor
bool endAll = false; // ends all infinite loops

// Handles parsing and errors
void parse( int argcount, char *argval[], char* &host_name, char* &port_n, char* &host_dir )
{
    // Display help message if the number of arguements are incorrect
    if ( argcount != 4 )
    {
        // We print argv[0] assuming it is the program name
        cout << "Usage: "<< argval[0] << " <hostname> <port> <hosting_directory>" << endl;
        exit(0);
    }
    
    // Parse values
    else 
    {
        // argv[1] is the hostname
        host_name = argval[1];

        // argv[2] is the port number
        port_n = argval[2];
        int Port = atoi(argval[2]);
        if(Port <= 0 || Port > 65535)
        {
            cerr << "Error: port number is invalid" << endl;
            exit(1);
        }

        // argv[3] is the hosting directory
        host_dir = argval[3];
    }
}

// ToDo:
// Multithreaded handler for each client
void clientHandler (int fileDsc) 
{
    char* req;          // Request buffer
    int reqlen = 0;     // Length of the buffer

    // Obtain the Request, and parse it using the user defined class
    receiveMessage(fileDsc, req, &reqlen, endAll);
    HttpGetRequest o;
    o.parseReq(req);
    
    /*
    // Debugging only: Print out the information of the obtained Request
    cout << "Length: " << reqlen << endl;
    cout << "Path: " << o.getPath() << endl;
    cout << endl << "Host: " << o.getHost() << endl;
    cout << endl << "Protocol Version: " << o.getProtocolVersion() << endl;
    cout << endl << "Generated HttpRequest: " << endl << o.genReq();
    */
    
    // Check for errors in the request
    // ToDo

    // Send the response
    string resp ("HTTP/1.1 200 OK\r\nDate: Sun, 03 Apr 2011 19:48:33 GMT\r\nServer: Apache/1.2.5\r\nLast-Modified: Tue, 22 Jun 2010 19:20:37 GMT\r\nETag: \"2b3e-258f-4c210d05\"\r\nContent-Length: 5\r\nAccept-Ranges: bytes\r\nContent-Type: text/html\r\n\r\nHello");
    sendMessage(fileDsc, resp.c_str(), resp.length());

    // Close the file descriptor for this instance
    close(fileDsc); 
}

// Initialize socket, and return the file descriptor 
void initializeSocket(char* &hostN, char* &portN, int *socketDesc)
{   
    // Socket addresses; I don't know if I can move these into initialize()
    AddressInfo hints, *servInfo, *p;    // getaddrinfo structs

    // Socket Procedures:
    // Zero out the hints, and set its values
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;          // Assuming IPv4
    hints.ai_socktype = SOCK_STREAM;    //TCP socket

    if ((getaddrinfo(hostN, portN, &hints, &servInfo) != 0)) 
    {
        cerr << "Call to getaddrinfo failed; check hostname and/or port number" << endl;
        exit(1);
    }

    // Loop through getaddrinfo results, until connection has been established
    for(p = servInfo; p != NULL; p = p->ai_next) 
    {
        if ((*socketDesc = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) 
        {
            // cerr << "Socket could not establish file descriptor" << endl;
            continue;
        }

        if (bind(*socketDesc, p->ai_addr, p->ai_addrlen) == -1) 
        {
            // cerr << "Could not connect to socket" << endl;
            close(*socketDesc);
            continue;
        }

        break; // if we get here, we must have connected successfully
    }

    // What if we couldn't connect with any host in the servinfo list?
    if (p == NULL) 
    {
        cerr << "Failed to connect to server" << endl ;
        close(*socketDesc);
        exit(1);
    } 

    freeaddrinfo(servInfo); // all done with this structure
}

void listenLoop (int socketDesc) 
{
    int incFileDesc;                     // Descriptor for each established connection
    socklen_t temp;                      // Temp for sizeof(clientAdd)
    SocketAddress clientAdd;             // One day, I will figure out why we need this

    // Listen for incoming connections. Queue up to 5 requests     
    listen(socketDesc, 5);

    // Multithread after this:
    // Continuously accept connections, and create a new clientHandler thread for each connection
    while (!endAll) {

        // Set the file descriptor to 
        temp = sizeof(clientAdd);
        incFileDesc = accept(socketDesc, (struct sockaddr *) &clientAdd, &temp);
        if (incFileDesc < 0) 
        {
            cerr << "Could not accept connection" << endl;
            close(socketDesc);
            exit(0);
        }

        thread handler = thread(clientHandler, incFileDesc);
        handler.detach(); // run in background, destroy automatically on end

        // Debugging only: Print if successful
        cout << "Successfully obtained descriptor" << endl;
    }
}

// Interrupt signal handler
void interruptHandler (int signum) 
{
    cout << "Interrupted! All descriptors closed" << endl;
    endAll = true;      // End all infinite loops
    // Don't we need to join the threads here?
    close(socketDsc);   // Close socket to interrupt the socket accept() function
    exit(0);
}

int main ( int argc, char *argv[] )
{
    // Variable declarations
    char* hostName;     // IP or Domain that we need to connect to 
    char* hostDir;      // Path of directory to host on the server
    char* port;         // Port number to open socket

    // Parse the input
    parse(argc, argv, hostName, port, hostDir);

    // Initialize the scoket desctiptor
    initializeSocket(hostName, port, &socketDsc);

    // Register signal SIGINT and signal handler  
    signal(SIGINT, interruptHandler); 

    // This handles connections to the server
    listenLoop(socketDsc);

    // Loop exited on its own, must have run into error
    close(socketDsc);

    /*
    //Debugging only: Test if the descriptor can write
    char buffer[32];
    strcpy(buffer, "Hello, Socket World!");
    int n = write(FileDsc,buffer,strlen(buffer));
    if (n < 0) 
        cout << "Error writing to socket" << endl;
    */

    /*
    //Debugging only: Check if the parsing was done correctly
    cout << "Host: " << hostName << endl;
    cout << "Port: " << port << endl;
    cout << "Hosting Directory: " << hostDir << endl;
    */

    // Avoid errors
    return 0;
}