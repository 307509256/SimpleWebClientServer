#include "HttpRequest.h"
#include <iostream>
#include <string>
using namespace std;

HttpGetRequest::HttpGetRequest()
{
	this->path = NULL;
	this->host = NULL;
	strcpy(this->protocolVersion, "HTTP/1.0");
}

HttpGetRequest::HttpGetRequest(char* fpath, char* fhost)
{
	this->path = new char [strlen(fpath)];
	strcpy(this->path, fpath);
	this->host = new char [strlen(fhost)];
	strcpy(this->host, fhost);
	strcpy(this->protocolVersion, "HTTP/1.0");
}

char* HttpGetRequest::genReq()
{
	std::string get = "";
	get += "GET ";
	get += this->path;
	get += " ";
	get += this->protocolVersion;
	get += "\r\nHost: ";
	get += this->host;
	get += "\r\n\r\n";
	this->getReq = new char [get.length()];
	std::strcpy(this->getReq, get.c_str());
	return this->getReq;
}

//  ToDo:
void HttpGetRequest::parseReq(char* buffer) 
{

}

// Clear members
void HttpGetRequest::clear()
{
	delete this->path;
	delete this->host;
	std::strcpy(this->protocolVersion[9], "HTTP/1.0");
	delete this->getReq;

	this->path = NULL;
	this->host = NULL;
	this->getReq = NULL;
}

// Access methods
char* HttpGetRequest::getPath() 
{
	return this->path;
}

char* HttpGetRequest::getHost() 
{
	return this->host;
}

char* HttpGetRequest::getProtocolVersion() 
{
	return this->protocolVersion;
}
