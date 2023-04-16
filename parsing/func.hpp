
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
data_reader *parec(char *s);
std::vector<std::string> parser_helper(std::string s);
void pars_request(Parsed *data);
void pars_locations(Parsed *data);
server *data_handler(data_reader *s);

#endif