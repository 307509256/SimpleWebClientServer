// C++ headers
#include <fstream>
#include <iostream>
#include <cstring>
#include <cstdlib>

// Kernel Includes
#include <sys/socket.h>
#include <sys/types.h> 
#include <netinet/in.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <netdb.h>
#include <unistd.h>
#include <string>

using namespace std;

typedef struct addrinfo AddressInfo;

// Same story with these; I dunno if I can move this into returnDesc()
AddressInfo hints;                          // Address information of server to connect to
AddressInfo *servinfo, *p;                  // servinfo - linked list of possible servers

// Handles parsing and errors
void parse(int argcount, char* argval[], char* urls[], char* ports[], char* paths[]) {
    // Display help message if the number of arguments are incorrect
    if (argcount == 1) {
        // argval[0] is the program name
        cout << "Usage: " << argval[0] << " <URL> <URL> ..." << endl;
        exit(0);
    }

	// Temporary holder for each argument
	char* arg;

    // argval[1] through argval[arcount-1] are the desired URLs
    for (int i = 1; i < argcount; i++) {
    	// Get the i-th argument (after the program name)
		arg = argval[i];
		unsigned int len = strlen(arg);

		// Remove http:// from start of argument
		char temp[8];
		unsigned int j = 0;
		for (; j < 7; j++) {
			temp[j] = arg[j];
		}
		temp[7] = '\0';
		if (strcmp(temp, "http://\0"))
			j = 0;

		// Temporary helper variables
		int u = 0;
		char url[len] = "";
		int p = 0;
		char port[6] = "";
		int h = 0;
		char path[len] = "";

		// Parse the host name and port number
		for (; j < len; j++) {
			// Find start of port number
			if (arg[j] == ':' && arg[j+1] != '\0' && isdigit(arg[j+1])) {
				j++;
				// Extract port #
				while (arg[j] != '/' && arg[j] != '\0')
					port[p++] = arg[j++];
				if (arg[j] == '/')
					j--;
			}
			// If no port number and end of host name
			else if (arg[j] == '/')
				// Parse the path
				while (arg[j] != '\0')
					path[h++] = arg[j++];
			// Otherwise, add element to host name
			else
				url[u++] = arg[j];
		}

		// Null-terminate and remove unneccesary slashes
		if (url[u-1] == '/')
			url[u-1] = '\0';
		else
			url[u] = '\0';
		// Default value for port #
		if (!strcmp(port, ""))
			strcpy(port, "4000");
		else
			port[p] = '\0';
		// Default value for path
		if (!strcmp(path, ""))
			strcpy(path, "/\0");
		else
			path[h] = '\0';

		// Allocate space for elements
		urls[i-1] = new char [strlen(url)];
		paths[i-1] = new char [strlen(path)];
		ports[i-1] = new char [6];
		// Add parsed argument to respective arrays
		strcpy(urls[i-1], url);
		strcpy(ports[i-1], port);
		strcpy(paths[i-1], path);
	}
}

void returnDesc(char* &portN, char* &hostN, int *socketDes) {
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
}

void cleanConnection(int *socketDes) {
    // Done with this structure
    freeaddrinfo(servinfo);
    // Close socket
    close(*socketDes);
}

int sendmessage (int clientSockfd, const char* msg, int len) {
    cout << "before send" << endl;
    if (send(clientSockfd, msg, len, MSG_NOSIGNAL) == -1) {
      perror("send");
      return 6;
    }
    cout << "after send" << endl;
    return 0;
}

int main (int argc, char *argv[]) 
{
    // Variable declarations
    int sockfd;                                 // File descriptor for the socket

	char** urls = new char* [argc-1];
	char** paths = new char* [argc-1];
	char** ports = new char* [argc-1];

    // Parse the input
    parse(argc, argv, urls, ports, paths);
	cout << urls[0] << endl << ports[0] << endl << paths[0] << endl;
	//	cout << urls[1] << endl << ports[1] << endl << paths[1] << endl;

	string msg ("Hello world");
	string closemsg ("close");

    // Initialize the connection
    for (int i = 0; i < argc-1; i++) {
    	returnDesc(ports[i], urls[i], &sockfd);

   		cout << "start" << endl;

    	sendmessage(sockfd, msg.c_str(), msg.length());
    	sendmessage(sockfd, closemsg.c_str(), closemsg.length());

   		cout << "Success!" << endl;

    	// Close the connection
    	cleanConnection(&sockfd);
    }
    return 0;
}
