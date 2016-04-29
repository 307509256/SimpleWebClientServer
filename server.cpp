// C++ headers
#include <fstream>
#include <iostream>
#include <cstring>
#include <cstdlib>

// Kernel Includes1
#include <sys/socket.h>
#include <sys/types.h> 
#include <netinet/in.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <netdb.h>
#include <unistd.h>

using namespace std;

typedef struct sockaddr_in SocketAddress;

// Socket addresses; I don't know if I can move these into initialize()
SocketAddress serverAdd;             // Socket data structure for AF_INET - server and client
SocketAddress clientAdd;             // Set client addresses

// Handles parsing and errors
void parse( int argcount, char *argval[], char* &host_name, int &Port, char* &host_dir )
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
        Port = atoi(argval[2]);
        if(Port < 0 || Port > 65535)
        {
            cerr << "Error: port number is invalid" << endl;
            exit(1);
        }

        // argv[3] is the hosting directory
        host_dir = argval[3];
    }
}

// Initialize socket, and return the file descriptor 
void initializeSocket(int portNum, int *socketDesc, int *incFileDesc)
{   
    socklen_t temp;

    // Socket Procedures:
    // Start socket:      IPv4        TCP   Protocol
    *socketDesc = socket(AF_INET, SOCK_STREAM, 0);
    if(*socketDesc < 0)
    {
        cerr << "Error creating the socket." << endl;
        exit(0);
    }

    // Zero out server address 
    memset(&serverAdd, 0, sizeof(serverAdd));

    // Bind socket:
    // Set address values
    serverAdd.sin_family = AF_INET;
    // Set server addresses to bind to any ip address as set by the network
    serverAdd.sin_addr.s_addr = INADDR_ANY;
    // Set server bind port number
    serverAdd.sin_port = htons(portNum);
    // Bind socket
    if (bind(*socketDesc, (struct sockaddr *) &serverAdd, sizeof(serverAdd)) < 0) 
    {
        cerr << "Could not bind socket to port." << endl;
        exit(0);
    }

    // Listen for incoming connections. Queue up to 3 requests     
    listen(*socketDesc, 5);
    
    // Set the file descriptor to 
    temp = sizeof(clientAdd);
    *incFileDesc = accept(*socketDesc, (struct sockaddr *) &clientAdd, &temp);
    if (*incFileDesc < 0) 
    {
        cerr << "Could not accept connection" << endl;
        exit(0);
    }
}

void closeSocketAndDescriptor( int *socket, int *file)
{
    close(*file);
    close(*socket);
}

int main ( int argc, char *argv[] )
{
    // Variable declarations
    char* hostName;                             // IP or Domain that we need to connect to 
    char* hostDir;                              // Path of directory to host on the server
    int port;                                   // Port number to open socket
    int socketDsc, FileDsc;                     // Socket descriptor, and file descriptor

    // Parse the input
    parse(argc, argv, hostName, port, hostDir);

    // Grab the file descriptor      (This is the one)
    initializeSocket(port, &socketDsc, &FileDsc);
    // Close the connection
    closeSocketAndDescriptor(&socketDsc, &FileDsc);

    // Print if successful
    cout << "Successfully obtained descriptor" << endl;

    /*
    //Check if the parsing was done correctly
    cout << "Host: " << hostName << endl;
    cout << "Port: " << port << endl;
    cout << "Hosting Directory: " << hostDir << endl;
    */

}