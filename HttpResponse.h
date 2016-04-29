#ifndef __HTTPRESP__
#define __HTTPRESP__

#include "Http.h"

class HttpResponse: public Http
{
private:
	std::string contentType;
	double protocolVersion, contentLength;
	int statusCode;
	std::vector<std::string> data;

public:
	//Default Constructor
	HttpResponse();

	//Parameterized Constructor
	//HttpResponse(double ProtocolVersion, int StatusCode, double ContentLength, std::string contentType);

	//Get Methods
	std::string getContentType();
	double getProtocolVersion();
	double getContentLength();
	int getStatusCode();
	//getdata?

	//Set Methods
	void setContentType(std::string setVal);
	void setProtocolVersion(double setVal);
	void setContentLength(double setVal);
	void getStatusCode(int setVal);
	//setdata?
};

#endif