#include "server.hpp"


std::vector<std::string> copyy(std::vector<std::string> &x)
{
	std::vector<std::string> res;
	for (std::vector<std::string>::iterator it = x.begin(); it < x.end(); it++)
		res.push_back(*it);
	return res;
}

server &server::operator=(server &x)
{
	std::cout << "copy assignement operator\n";
	autoindex = x.autoindex;
	chunked_transfer_encoding = x.chunked_transfer_encoding;
	server_name = x.server_name;
	client_max_body_size = x.client_max_body_size;
	root = x.root;
	
	listen.first = x.listen.first;
	listen.second = x.listen.second;
	index = copyy(x.index);
	allow = copyy(x.allow);
	deny = copyy(x.deny);
	int i = 0;
	for (std::vector<Location>::iterator it = x.location.begin(); it < x.location.end();it++){
		location.push_back(Location());
		location[i] = *it;
		i++;
	}
	for (std::vector<std::pair<std::vector<std::string>, std::string> >::iterator it = x.error_page.begin(); it < x.error_page.end(); it++)
	{
		std::pair<std::vector<std::string>, std::string>	pr;
		std::vector<std::string>							v;
		for	(std::vector<std::string>::iterator it2 = (*it).first.begin(); it2 < (*it).first.end(); it2++)
			v.push_back(*it2);
		pr.first = v;
		pr.second = (*it).second;
		this->error_page.push_back(pr);
	}
	return *this;
}