
#ifndef PARS_HPP
#define PARS_HPP
#include "includes.hpp"

class request;
class data_reader;

class Parsed
{
private:
	std::vector<data_reader> s;
	std::vector<server> handled_data;

public:
	request *req;
	Parsed();
	Parsed &operator=(const Parsed &parsed);
	Parsed(const Parsed &s);
	std::vector<data_reader> getserver() const;
	std::vector<server> getDate() const;
	Parsed(char *);
	// ~Parsed();
};

#endif