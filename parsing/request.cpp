#include "request.hpp"

request::request(std::string x, int *status)
{
	std::ifstream myfile;
	std::string tmp;
	int i = 0;
	myfile.open(x);
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
		else if (tmp.find(":") != std::string::npos)
		{
			std::string tmp2;
			std::string tmp3;
			iss >> tmp2;
			iss >> tmp3;
			std::pair<std::string, std::string> pr;
			pr.first = tmp2;
			pr.second = tmp3;
			data.insert(pr);
		}
		else{
			body << tmp;
			// std::cout << tmp << std::endl;
		}
		i++;
	}
	std::cout << ">>>>>>>>>>> start of bady" << body.str() << "   end of body<<<<<<<<<<" << std::endl;
}