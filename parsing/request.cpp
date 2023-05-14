#include "request.hpp"

request::request(int fd, int *status, int *read_len)
{
	std::ifstream myfile;
	std::ofstream myfile1;
	std::ostringstream  ss1;
	std::ostringstream  ss2;
	std::string tmp;
	int i = 0;
	ss1 << "/tmp/request_" << fd;
	ss2 << "/tmp/response_" << fd;
	myfile.open(ss1.str());
	if (myfile.fail())
		exit(0);
	while (getline(myfile, tmp))
	{
		std::istringstream iss(tmp);
		if (tmp == "\r") // this is the end of request
		{
			*status = 1;
			// std::cout << ">>>.<<<" << std::endl;
		}
		else if (i == 0)
		{
			iss >> method;
			iss >> absoluteURI;
			iss >> http_version;
		}
		else if (tmp.find(":") != std::string::npos)// && tmp.find("<") == std::string::npos)
		{
			std::string tmp2;
			std::string tmp3;
			iss >> tmp2;
			iss >> tmp3;
			std::pair<std::string, std::string> pr;
			pr.first = tmp2;
			pr.second = tmp3;
			data.insert(pr);
			if (tmp2 == "Content-length")
				*read_len = stoi(tmp3);

		}
		else{
			myfile.open(ss1.str());
			// if (myfile.fail())
			// 	exit(0);
			myfile1 << tmp;
		// 	// std::cout << tmp << std::endl;
		}
		i++;
	}
	// std::cout << ">>>>>>>>>>> start of bady" << body << "   end of body<<<<<<<<<<" << std::endl;
}
request::request(){}