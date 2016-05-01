#ifndef __HTTPRESP__
#define __HTTPRESP__

#include <cstring>
#include <string>
<<<<<<< HEAD

class HttpResponse
{
private:
	char[9] protocolVersion;
	double contentLength;
	int statusCode;
	char *status;
	char* payload;
	char* resquest;

=======
#include <cstdlib>
#include <iostream>
#include <sstream>

class HttpResponse
{
protected:
	// Data members
	unsigned int contentLength;
	int statusCode;
	char* status;
	char* payload;
	char* response;
	char* protocolVersion;
	
	// Protected  helper functions
	// Generate status message
	void genStatus(int statusC);
	// Parsing Helper
	void helper(char* buffer, char* &dataToStore);
	// Clear all pointers and buffers
	void clear();
>>>>>>> devin

public:
	// Default Constructor
	HttpResponse();
	// Parameterized Constructor 
	HttpResponse(double payloadLen, int StatCode, char* htmlPayload);
<<<<<<< HEAD
	// Generate status message
	void genStatus(int statusC);
	// Generate the HTTP request
	char* genReq();
	// Clear all pointers and buffers
	void clear();
	// Parse request
	void parseReq(char *buffer);

	// Get methods
	
=======
	// Destructor
	~HttpResponse();

	// Generate the HTTP request
	char* genReq();
	// Parse request
	void parseReq(char *buffer);
	
	// Get methods
	unsigned int getContentLength();
	int getStatusCode();
	char* getStatus();
	char* getPayload();
	char* getProtocolVersion();
>>>>>>> devin
};

#endif