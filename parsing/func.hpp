#ifndef FUNC_HPP
#define FUNC_HPP
#include "includes.hpp"
class server;
struct data_reader;
class Location;
class Parsed;
struct client;
class request;
struct data_reader
{
public:
	data_reader &operator=(data_reader &);
	std::string block_name;
	std::vector<std::string> dir;
	std::vector<data_reader> block;
};

data_reader read_block(std::ifstream &myFile, std::string block_start);
std::vector<data_reader> parec(char *s);
std::vector<std::string> parser_helper(std::string s);
request *pars_request(client *cl, std::string s);
std::vector<Location> pars_locations(data_reader data);
std::vector<server> data_handler(std::vector<data_reader> s);

#endif