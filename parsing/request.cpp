#include "request.hpp"

request::request(std::string x)
{
	std::ifstream myfile;
	std::string tmp;
	int i = 0;
	myfile.open(x);
	if (myfile.fail())
	{
		// error msg;
	}
	while (getline(myfile, tmp))
	{
		std::istringstream iss(tmp);
		if (i == 0)
		{
			iss >> method;
			iss >> absoluteURI;
			iss >> http_version;
		}
		else
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
		i++;
	}
}