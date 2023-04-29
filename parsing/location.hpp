#ifndef LOCATION_HPP
#define LOCATION_HPP
#include "includes.hpp"
class location
{
public:
	std::vector<std::pair<std::vector<std::string>, std::string> > error_page;
	std::string client_max_body_size;
	// std::vector<std::string> index;
	std::vector<std::vector<std::string> > try_files;
	std::string autoindex;
	std::string chunked_transfer_encoding;
	location	&operator=(location &x);
};



#endif