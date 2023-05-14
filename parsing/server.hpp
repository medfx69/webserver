#ifndef SERVER_HPP
#define SERVER_HPP
#include "includes.hpp"

std::vector<std::string> copyy(std::vector<std::string> &x);

class Location;

class server
{
	public:
		std::vector<Location> location;
		std::vector<std::string> methods;
		std::vector<std::pair<std::string, std::string> > listen;
		std::string server_name;
		std::vector<std::pair<std::vector<std::string>, std::string> > error_page;
		std::string client_max_body_size;
		std::string root;
		std::vector<std::string> index;
		std::vector<std::string> allow;
		std::vector<std::string> deny;
		std::string autoindex;
		std::string chunked_transfer_encoding;
		server &operator=(server &x);
};

#endif