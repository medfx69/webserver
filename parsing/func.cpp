#include "func.hpp"

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
		else
			;
	}
	return s;
}

data_reader *parec(char *s)
{
	data_reader *ser;
	std::string file(s);
	std::ifstream myFile(file);
	std::string readed;

	ser = new data_reader();
	while (getline(myFile, readed) && readed.find('{') == std::string::npos)
		;
	ser->block_name = readed.substr(0, readed.find('{'));
	while (getline(myFile, readed))
	{
		if (readed.find('{') != std::string::npos)
			ser->block.push_back(read_block(myFile, readed));
		else if (readed.find(';') != std::string::npos)
			ser->dir.push_back(readed);
		else
			;
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
		ret.push_back(tmp);
	return ret;
}

void pars_request(Parsed *data)
{
	// std::ifstream myfile;
	// std::string tmp;
	// int i = 0;
	data->req = new request("usefull_files/request");
	// myfile.open("usefull_files/request");
	// if (myfile.fail())
	// {
	// }
	// while (getline(myfile, tmp))
	// {
	// 	std::istringstream iss(tmp);
	// 	if (i == 0)
	// 	{
	// 		iss >> data->req->method;
	// 		iss >> data->req->absoluteURI;
	// 		iss >> data->req->http_version;
	// 	}
	// 	else
	// 	{
	// 		std::string tmp2;
	// 		std::string tmp3;
	// 		iss >> tmp2;
	// 		iss >> tmp3;
	// 		std::pair<std::string, std::string> pr;
	// 		pr.first = tmp2;
	// 		pr.second = tmp3;
	// 		data->req->data.insert(pr);
	// 	}
	// 	i++;
	// }
}

void pars_locations(Parsed *data)
{
	std::vector<data_reader>::iterator it = data->getserver()->block.begin();

	while (it < data->getserver()->block.end())
	{
		std::istringstream iss((*it).block_name);
		std::string tmp;
		iss >> tmp;
		iss >> tmp;
		(*it).block_name = tmp;
		std::vector<std::string>::iterator it2 = (*it).dir.begin();
		location *x = new location();
		while (it2 < (*it).dir.end())
		{
			std::istringstream iss2(*it2);
			std::string tmp;
			std::string tmp2;

			iss2 >> tmp;
			iss2 >> tmp2;
			if (tmp2.find(';') != std::string::npos)
				tmp2.erase(tmp2.find(';'), 1);

			if (tmp.compare("try_files") == 0)
				x->try_files.push_back(parser_helper(*it2));
			else if (tmp.compare("client_max_body_size") == 0)
				x->client_max_body_size = tmp2;
			else if (tmp.compare("autoindex") == 0)
				x->autoindex = tmp2;
			else if (tmp.compare("chunked_transfer_encoding") == 0)
				x->chunked_transfer_encoding = tmp2;
			else if (tmp.compare("error_page") == 0)
			{
				std::pair<std::vector<std::string>, std::string> adder;
				adder.first = parser_helper(*it2);
				while (iss2 >> tmp2)
					;
				if (tmp2.find(';') != std::string::npos)
					tmp2.erase(tmp2.find(';'), 1);
				adder.second = tmp2;
				x->error_page.push_back(adder);
				// std::cout << "Vector of pair the first :: " << std::endl;
				// std::vector<std::string>::iterator itt = adder.first.begin();
				// while (itt < adder.first.end())
				// {
				//     std::cout << "this is vector first element  -> " << (*itt) << std::endl;

				//     itt++;
				// }
				// std::cout << "this is second -> " << adder.second << std::endl;
			}

			it2++;
		}
		it++;
	}
}

server *data_handler(data_reader *s)
{
	std::vector<std::string>::iterator it = s->dir.begin();

	server *x = new (server);

	while (it < s->dir.end())
	{
		std::istringstream iss(*it);
		std::string tmp;
		std::string tmp2;

		iss >> tmp;
		iss >> tmp2;
		if (tmp2.find(';') != std::string::npos)
			tmp2.erase(tmp2.find(';'), 1);

		if (tmp.compare("server_name") == 0)
			x->server_name = tmp2;
		else if (tmp.compare("listen") == 0)
		{
			tmp = tmp2;
			x->listen.first = tmp.substr(0, tmp.find(':'));
			x->listen.second = tmp.substr(tmp.find(':') + 1, tmp.find(';') - tmp.find(':') - 1);
		}
		else if (tmp.compare("root") == 0)
			x->root = tmp2;
		else if (tmp.compare("client_max_body_size") == 0)
			x->client_max_body_size = tmp2;
		else if (tmp.compare("autoindex") == 0)
			x->autoindex = tmp2;
		else if (tmp.compare("chunked_transfer_encoding") == 0)
			x->chunked_transfer_encoding = tmp2;
		it++;
	}
	return (x);
}