#include "HttpResponse.h"

HttpResponse::HttpResponse()
{
	this->contentLength = NULL;
	this->statusCode = NULL;
	this->payload = NULL;
	this->request = NULL;
	std::strcpy(this->protocolVersion, "HTTP/1.0");
}

// Default Constructor
HttpResponse::HttpResponse(double payloadLen, int StatCode, char* htmlPayload)
{
	this->contentLength = payloadLen;
	this->statusCode = StatCode;
	this->status = this->genStatus(statusCode);
	this->genStatus(this->status);
	this->payload = new char [std::strlen(htmlPayload)];
	std::strcpy(this->payload, htmlPayload);
}

// Clean up pointers and zero out data
void HttpResponse::clear()
{
	std::strcpy(protocolVersion, "HTTP/1.0");
	this->contentLength = 0;
	this->statusCode = 0;
	delete this->status;
	delete this->payload;
	delete this->resquest;
	this->status = NULL;
	this->payload = NULL;
	this->request = NULL;
}

// Get the response code string
void HttpResponse::genStatus(int statusC)
{
	if (statusC == 200)
	{
		this->status = new char [3];
		std::strcpy(this->status, "OK");	 
	}
	else if(statusC == 400)
	{
		this->status = new char [12];
		std::strcpy(this->status, "Bad request");
	}
	else
	{
		this->status = new char[10];
		std::strcpy(this->status, "Not found")
	}
}

// Generate the HTTP request
char* HttpResponse::genReq()
{
	std::string temp = "";
	temp += this->protocolVersion; //"HTTP/1.0";
	temp += this->statusCode;
	temp += " ";
	temp += this->status; 
	temp += "\r\n";
	temp += "Content-Length: "; 
	temp += contentLength;
	temp += "\r\n\r\n";
	temp += payload;
	this->request = new char [temp.length()];
	std::strcpy(this->request, temp.c_str());
	return this->request;
}

void parseReq(char *buffer)
{
	// ToDo:
}