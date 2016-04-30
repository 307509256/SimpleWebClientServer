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

typedef struct addrinfo AddressInfo;


// Handles parsing and errors
void parse( int argcount, char *argval[], char* &host_name, char* &port_n)
{
    int Port;
    // Display help message if the number of arguements are incorrect
    if ( argcount != 3 )
    {
        // We print argv[0] assuming it is the program name
        cout << "Usage: " << argval[0] << " <hostname> <port>" << endl;
        exit(0);
    }
    // Parse values
    else 
    {    
        // argv[1] is the hostname
        host_name = argval[1];
        if (host_name == NULL) 
        {
            cerr << "Hostname is invalid" << endl;
            exit(0);
        }
        // argv[2] is the port number
        port_n = argval[2];
        Port = atoi(argval[2]);
        if(Port < 0 || Port > 65535)
        {
            cerr << "Error: port number is invalid" << endl;
            exit(1);
        }
    }
}

void returnDesc(char* &portN, char* &hostN, int *socketDes)
{
    // Same story with these; I dunno if I can move this into returnDesc()
    AddressInfo hints;                          // Address information of server to connect to
    AddressInfo *servinfo, *p;                  // sevinfo - linked list of possible servers

    // Zero out the hints
    memset(&hints, 0, sizeof hints);

    // Set the struct values
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    // Call getaddrinfo
    if (getaddrinfo(hostN, portN, &hints, &servinfo) != 0) 
    {
        cerr << "Call to getaddrinfo failed; check hostname and/or port number" << endl;
        exit(1);
    }

    // Loop through getaddrinfo results, until connection has been established
    for(p = servinfo; p != NULL; p = p->ai_next) 
    {
        if ((*socketDes = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) 
        {
            // cerr << "Socket could not establish file descriptor" << endl;
            continue;
        }

        if (connect(*socketDes, p->ai_addr, p->ai_addrlen) == -1) 
        {
            // cerr << "Could not connect to socket" << endl;
            close(*socketDes);
            continue;
        }

        break; // If we reach the break, we have a valid socketfd
    }

    // What if we couldn't connect with any host in the servinfo list?
    if (p == NULL) 
    {
        cerr << "Failed to connect to server" << endl ;
        exit(1);
    } 

    // Done with this structure
    freeaddrinfo(servinfo);
}

void cleanConnection(int *socketDes)
{
    // Close socket
    close(*socketDes);
}

int main ( int argc, char *argv[] )
{
    // Variable declarations
    char* hostName;                             // IP or Domain that we need to connect to 
    char* port;                                   // Port number to connect to
    char* hostDir;                              // Message to send to the server (HTTP GET ?)
    int sockfd;                                 // File descriptor for the socket

    // Parse the input
    parse(argc, argv, hostName, port);

    // Initialize the connection
    returnDesc(port, hostName, &sockfd);
    
    // We can now read and write from sockfd
    
    /*
    //Test if the socket is reading
    char buffer[32];
    int n = read(sockfd, buffer, 31);
    if (n < 0) 
        cout << "Error reading from socket" << endl;
    cout << buffer << endl;
    */
    
    // Close the connection
    cleanConnection(&sockfd);

    cout << "Success!" << endl;

    return 0;

}