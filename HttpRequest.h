#ifndef __HTTPREQ__
#define __HTTPREQ__

#include <cstring>
#include <string>

<<<<<<< HEAD
class HttpGetRequest {
protected:
	char* path;
	char* host;
	char protocolVersion[9];
	char* getReq;
=======
class HttpGetRequest 
{
protected:
	// Protected datamembers
	char* path;
	char* host;
	char* protocolVersion;
	char* getReq;
	
	// Helper fuction
	void helper(char* buffer, char* &dataToStore);
	// Clear cstrings
	void clear();
>>>>>>> devin

public:
	// Default Constructor
	HttpGetRequest();
	// Parameterized Constructor
	HttpGetRequest(char* fpath, char* fhost);
<<<<<<< HEAD
=======
	// Destructor
	~HttpGetRequest();
>>>>>>> devin
	// Generate the HTTP request
	char* genReq();
	// Parse request
	void parseReq(char *buffer);
<<<<<<< HEAD
	// Clear cstrings
	void clear();
=======
>>>>>>> devin

  	// Return protected variables
	char* getPath();
  	char* getHost();
  	char* getProtocolVersion();
<<<<<<< HEAD
=======
  	// int getErrorStatus();
>>>>>>> devin
};

#endif
