#include "func.hpp"

data_reader &data_reader::operator=(data_reader& data)
{
	block_name = data.block_name;
	dir = copyy(data.dir);
	std::cout << "sdffdsfadsfads" << std::endl;
	// kfor (int i = 0; i < data.block.size(); i++)
	// {
	// 	// std::cout << ">>>>";
	// 	std::cout << ">>>>" << data.block[i].block_name << std::endl;
	// }
		
	return *this;
}

int is_spaces(std::string s)
{
	for (size_t i = 0; i < s.length(); i++)
	{
		if (!isspace(s[i]))
			return 0;
	}
	return 1;
}

data_reader read_block(std::ifstream &myFile, std::string block_start)
{
	data_reader s;
	std::string readed;

	s.block_name = block_start.substr(0, block_start.find('}'));
	while (getline(myFile, readed))
	{
		if (readed.find('}') != std::string::npos)
			return s;
		else if (readed.find(';') != std::string::npos)
			s.dir.push_back(readed);
		else if (!readed.empty() && !is_spaces(readed))
		{
			std::cerr << "Error: Bad config file.\n";
			exit(1);
		}
	}
	return s;
}

data_reader read_server_block(std::ifstream &myFile, std::string block_start)
{
	data_reader s;
	std::string readed;

	s.block_name = block_start.substr(0, block_start.find('}'));// here we can check if syntax is clear
	while (getline(myFile, readed))
	{
		if (readed.find('{') != std::string::npos)
	 		s.block.push_back(read_block(myFile, readed));
		else if (readed.find('}') != std::string::npos)
			return s;
		else if (readed.find(';') != std::string::npos)
			s.dir.push_back(readed);
		else if (!readed.empty())
		{
			std::cerr << "Error: Bad config file.\n";
			exit(1);
		}
	}
	return s;
}

// this function need to return a vector

std::vector<data_reader> parec(char *s)
{
	std::vector<data_reader> ser;
	std::string file(s);
	std::ifstream myFile(file);
	std::string readed;

	while (getline(myFile, readed))
	{
		if (readed.find("server {") != std::string::npos)
			ser.push_back(read_server_block(myFile, readed));
		else if (!readed.empty())
		{
			std::cerr << "Error: Bad config file.\n";
			exit(1);
		}
	}
	return ser;
}

std::vector<std::string> parser_helper(std::string s)
{
	std::vector<std::string> ret;
	std::istringstream iss(s);
	std::string tmp;

	iss >> tmp;
	while (iss >> tmp)
	{
		if (tmp.find(';') != std::string::npos)
				tmp.erase(tmp.find(';'), 1);
		ret.push_back(tmp);
	}
	return ret;
}

request *pars_request(Parsed data, int fd, int *status)
{
    std::ostringstream  ss1;

	ss1 << "/tmp/request_" << fd;
	data.req = new request(ss1.str(), status);
	return data.req;
}

std::vector<Location> pars_locations(data_reader data)
{
	std::vector<Location> locv;
	std::vector<data_reader>::iterator it0 = data.block.begin();
	while (it0 < data.block.end())
	{
		std::istringstream iss((*it0).block_name);
		std::string tmp;
		Location x;
		iss >> tmp;
		iss >> tmp;
		(*it0).block_name = tmp;
		x.location_name = (*it0).block_name;
		std::vector<std::string>::iterator iite2 = (*it0).dir.begin();
		while (iite2 < (*it0).dir.end())
		{
			std::istringstream iss2(*iite2);
			std::string tmp;
			std::string tmp2;

			iss2 >> tmp;
			iss2 >> tmp2;
			if (tmp2.find(';') != std::string::npos)
				tmp2.erase(tmp2.find(';'), 1);

			if (tmp.compare("try_files") == 0)
				x.try_files.push_back(parser_helper(*iite2));
			else if (tmp.compare("client_max_body_size") == 0)
				x.client_max_body_size = tmp2;
			else if (tmp.compare("autoindex") == 0)
				x.autoindex = tmp2;
			else if (tmp.compare("chunked_transfer_encoding") == 0)
				x.chunked_transfer_encoding = tmp2;
			else if (tmp.compare("error_page") == 0)
			{
				std::pair<std::vector<std::string>, std::string> adder;
				adder.first = parser_helper(*iite2);
				while (iss2 >> tmp2)
					;
				if (tmp2.find(';') != std::string::npos)
					tmp2.erase(tmp2.find(';'), 1);
				adder.second = tmp2;
				x.error_page.push_back(adder);
			}
			else if (!tmp.empty())
			{
				std::cerr << "Error: Bad config file.\n";
				exit(1);
			}
			iite2++;
		}
		it0++;
		locv.push_back(x);
	}
	return locv;
}

std::vector<server> data_handler(std::vector<data_reader> s)
{
	std::vector<server> serv;
	for(std::vector<data_reader>::iterator it0 = s.begin(); it0 < s.end(); it0++)
	{
		std::vector<std::string>::iterator it = (*it0).dir.begin();

		server x;

		while (it < (*it0).dir.end())
		{
			std::istringstream iss(*it);
			std::string tmp;
			std::string tmp2;

			iss >> tmp;
			iss >> tmp2;
			if (tmp2.find(';') != std::string::npos)
				tmp2.erase(tmp2.find(';'), 1);

			if (tmp.compare("server_name") == 0)
				x.server_name = tmp2;
			else if (tmp.compare("listen") == 0)
			{
				tmp = tmp2;
				x.listen.first = tmp.substr(0, tmp.find(':'));
				x.listen.second = tmp.substr(tmp.find(':') + 1, tmp.find(';') - tmp.find(':') - 1);
			}
			else if (tmp.compare("root") == 0)
				x.root = tmp2;
			else if (tmp.compare("client_max_body_size") == 0)
				x.client_max_body_size = tmp2;
			else if (tmp.compare("autoindex") == 0)
				x.autoindex = tmp2;
			else if (tmp.compare("chunked_transfer_encoding") == 0)
				x.chunked_transfer_encoding = tmp2;
			else if (tmp.compare("methods") == 0)
				x.methods = parser_helper(*it);
			else if (tmp.compare("error_page") == 0)
			{
				std::pair<std::vector<std::string>, std::string> adder;
				adder.first = parser_helper(*it);
				while (iss >> tmp2)
					;
				if (tmp2.find(';') != std::string::npos)
					tmp2.erase(tmp2.find(';'), 1);
				adder.second = tmp2;
				x.error_page.push_back(adder);
			}
			else if (!tmp.empty())
			{
				std::cerr << "Error: Bad config file.\n";
				exit(1);
			}
			it++;
		}
		x.location = pars_locations(*it0);
		serv.push_back(x);
	}
	return (serv);
}
