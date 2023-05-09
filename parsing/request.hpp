
#ifndef REQUEST_HPP
#define REQUEST_HPP
#include "includes.hpp"

class request
{
public:
	std::string method;
	std::string absoluteURI;
	std::string http_version;
	std::map<std::string, std::string> data;	
	request(std::string x, int *status);
};


#endif