#pragma once

#include "includes.hpp"
class Location
{
public:
	std::vector<std::pair<std::vector<std::string>, std::string> > error_page;
	std::string location_name;
	std::string client_max_body_size;
	// std::vector<std::string> index;
	std::vector<std::vector<std::string> > try_files;
	std::string autoindex;
	std::string chunked_transfer_encoding;
	Location	&operator=(Location &x);
};
