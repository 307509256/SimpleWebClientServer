#ifndef __HTTPRESP__
#define __HTTPRESP__

#include <cstring>
#include <string>

class HttpResponse
{
private:
	char[9] protocolVersion;
	double contentLength;
	int statusCode;
	char *status;
	char* payload;
	char* resquest;


public:
	// Default Constructor
	HttpResponse();
	// Parameterized Constructor 
	HttpResponse(double payloadLen, int StatCode, char* htmlPayload);
	// Generate status message
	void genStatus(int statusC);
	// Generate the HTTP request
	char* genReq();
	// Clear all pointers and buffers
	void clear();
	// Parse request
	void parseReq(char *buffer);

	// Get methods
	
};

#endif