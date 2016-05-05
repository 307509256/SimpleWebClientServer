#include "Helper.h"

// Send a message to the server/client
int sendMessage(int clientSockfd, const char* msg, int len) 
{
    // Send cstring; return error if nothing was sent
    if (send(clientSockfd, msg, len, MSG_NOSIGNAL) == -1) 
    {
        std::cerr << "Could not send requset" << std::endl;
        exit(1);
    }
    return 0;
}

// Receives a message from the server/client
int receiveMessage(int sockfd, char* &result, int* messageLen, int end) 
{
    // send/receive data to/from connection
    char buf[BUFFER_LEN] = {0};
    std::string raw = "";
    bool started = false;               // Just a flag, to make sure that data was sent
    int sleepcount = 0;
    int bytesRead = 0;                  // Used to count the amount of bytes sent over the descriptor

    while (!end) 
    {
        memset(buf, '\0', sizeof(buf));     // Zero out the buffer

        // Receive over the file descriptor
        if ((bytesRead = recv(sockfd, buf, BUFFER_LEN-1, 0)) == -1) 
        {
            std::cerr << "Failed to receive data from the server." << std::endl;
            exit(1);
        }

        // Check if data was transmitted
        if (!started && bytesRead > 0) 
            started = true;

        // Transfer the rest of the file
        if (started) 
        {
            raw += buf;

            // Finished reading; copy the buffer over
            if (bytesRead < BUFFER_LEN-1) 
            {
                if (sleepcount < 5) 
                {
                    usleep(1000);
                    sleepcount++;
                    continue;
                }
                /*
                // Debugging only: comment out
                cout << raw << endl;
                */

                raw += '\0'; // add null terminator
                
                result = new char[raw.length()];
                *messageLen = raw.length();
                std::strncpy(result, raw.c_str(), raw.length());
                return 0;
            } 
            else 
            {
                sleepcount = 0;
            }
        }
    }
    
    // Should never reach this point; but just in case...
    return 0;
}

// Checks if there is an error in the passed in string
int errorStatus(char* &path, char* protocolVersion)
{
    int flag = 0;

    // Its simple to treat / as 404
    if(std::strcmp(path, "/") == 0)
    {
        return 404;
    }
    // Else check if the file exists, like one usually would
    else
    {
        if(access(path, R_OK) == 0)
            flag = 200;
        else
            return 404;
    }

    // Check if the protocol version is valid
    if(std::strcmp(protocolVersion, "HTTP/1.0") == 0)
    {
        flag = 200;
    }
    else
    {
        return 400;
    }

    // Return the result
    return flag;
}

void getFilename(char* &filePath)
{
    std::string fPath(filePath);
    std::string temp = "";

    // Handle index.html
    if(fPath == "/")
    {
        fPath = "index.html";
        delete filePath;
        filePath = new char [fPath.length()];
        std::strcpy(filePath, fPath.c_str());
        return;
    }

    for(size_t i = (fPath.length() - 1); i > 0; i--)
    {
        if(fPath[i] != '/')
        {
            temp += fPath[i];
        }
        else
        {
            break;
        }
    }

    fPath = "";
    for(size_t i = 0; i < temp.length(); i++)
    {
        fPath[i] = temp[(temp.length()-1)-i];
    }
    fPath[temp.length()] = 0;

    delete filePath;
    filePath = new char [fPath.length()];
    std::strcpy(filePath, fPath.c_str());
}