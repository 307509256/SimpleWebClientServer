#ifndef __HTTPREQ__
#define __HTTPREQ__

#include <cstring>
#include <string>

class HttpGetRequest {
protected:
	char* path;
	char* host;
	char protocolVersion[9];
	char* getReq;

public:
	// Default Constructor
	HttpGetRequest();
	// Parameterized Constructor
	HttpGetRequest(char* fpath, char* fhost);
	// Generate the HTTP request
	char* genReq();
	// Parse request
	void parseReq(char *buffer);
	// Clear cstrings
	void clear();

  	// Return protected variables
	char* getPath();
  	char* getHost();
  	char* getProtocolVersion();
};

#endif
