#pragma once

#include "includes.hpp"
class Location
{
public:
	std::vector<std::pair<std::vector<std::string>, std::string> > error_page;
	std::string location_name;
	std::string root;
	std::string upload;
	std::string client_max_body_size;
	std::vector<std::vector<std::string> > try_files;
	std::string autoindex;
	std::vector<std::string> index;
	std::vector<std::string> methods;
	std::string chunked_transfer_encoding;
	std::string	cgi_path;
	Location	&operator=(Location &x);
};