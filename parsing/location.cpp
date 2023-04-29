#include "location.hpp"

location	&location::operator=(location &x)
{
	this->client_max_body_size = x.client_max_body_size;
	this->autoindex = x.autoindex;
	this->chunked_transfer_encoding = x.chunked_transfer_encoding;
	for (std::vector<std::vector<std::string> >::iterator it = x.try_files.begin();it < x.try_files.end();it++)
	{
		std::vector<std::string> v;
		for (std::vector<std::string>::iterator it2 = (*it).begin(); it2 < (*it).end(); it2++)
			v.push_back(*it2);
		this->try_files.push_back(v);
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