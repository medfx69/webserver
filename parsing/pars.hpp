
#ifndef PARS_HPP
#define PARS_HPP
#include "includes.hpp"

class request;
class data_reader;

class Parsed
{
private:
	data_reader *s;
	server *handled_data;

public:
	request *req;
	Parsed();
	Parsed &operator=(const Parsed &parsed);
	Parsed(const Parsed &s);
	data_reader *getserver() const;
	server *getDate() const;
	Parsed(char *);
	~Parsed();
};

#endif