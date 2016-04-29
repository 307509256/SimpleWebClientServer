#ifndef __HTTP__
#define __HTTP__

#include <sting>
#include <vector>

class Http
{
protected:
	//common elements
	float protocolVersion;

public:
	//get
	virtual float getProtocolVersion();

	//set
	virtual void setProtocolVersion(float setVal);
};

#endif