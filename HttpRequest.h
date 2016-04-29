#ifndef __HTTPREQ__
#define __HTTPREQ__

#include "Http.h"

class HttpGetRequest: public Http
{
protected: 
	std::string request, host,
	std::vector<std::string> accept;
	int timeout; 
	bool keepAlive;
public:
	//Default Constructor
	HttpRequest();

	//Parameterized Constructor
	//HttpRequest(std::string RequestLink, std::string Host, double ProtocolVersion, int TimeoutVal, float ProtocolVersion, bool KeepAlive);

	//Get methods
	std::string getRequest();
	std::string getHost();
	float getProtocolVersion();
	int getTimeout();
	bool isKeepAlive();
	//getAccept?

	//Set methods
	void setRequest(std::string setVal);
	void setHost(std::string setVal);
	void setProtocolVersion(float setVal);
	void setTimeout(int setVal);
	void setKeepAlive(bool setVal);
	//setAccept?
};

#endif