
#ifndef FUNC_HPP
#define FUNC_HPP
#include "includes.hpp"
class server;
class data_reader;
class Parsed;
struct data_reader
{
public:
	std::string block_name;
	std::vector<std::string> dir;
	std::vector<data_reader> block;
};

data_reader read_block(std::ifstream &myFile, std::string block_start);
std::vector<data_reader> parec(char *s);
std::vector<std::string> parser_helper(std::string s);
void pars_request(Parsed *data);
void pars_locations(Parsed *data);
std::vector<server> data_handler(std::vector<data_reader> s);

#endif