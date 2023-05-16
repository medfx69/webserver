#include "request.hpp"

void end_of_headers(request *req, client *cl)
{
	if (req->data.find("Transfer-Encoding:") != req->data.end())
	{
		if (req->data.find("Transfer-Encoding:")->second == "chunked")
			cl->chunked = 1;
	}
	if (req->data.find("Content-Length:") != req->data.end() && cl->chunked == 0)
	{
		try
		{
			std::cout << ">>>>>>>> " << req->data.find("Content-Length:")->second << std::endl;
			cl->read_len = stol(req->data.find("Content-Length:")->second);
		}
		catch (std::exception &x)
		{
			std::cout << x.what() << std::endl;
		}
	}
	if (cl->read_len == 0 && cl->chunked == 0)
		cl->read_status = 1;
	// std::cout << cl->read_len << " : " << cl->readed << " : " << cl->flag << std::endl;
}

// void	recursivness(std::string &s, std::ofstream myfile1, client *cl)
// {
// 	if (s.find("\r\n"))
// 	{

// 	}
// }

void request::handle_body(client *cl, std::string s)
{
	std::ostringstream ss2;
	std::ofstream myfile1;
	std::string tmp;

	ss2 << "/tmp/body_" << cl->client_fd;
	myfile1.open(ss2.str(), std::ofstream::app);
	if (cl->chunked == 0 && cl->read_len)
	{
		myfile1 << s;
		cl->readed += s.size();
		if (cl->readed >= cl->read_len)
		{
			cl->read_status = 1;
			cl->flag = 2;
		}
	}
	else
	{
		// std::cout << "eeehhooo :::::  " << s.size() << std::endl;
		// recursivness(s, myfile1, cl);
		std::stringstream ss1(s);
		while (getline(ss1, tmp))
		{
			// if (tmp.find("\r\n\r\n"))
			// 	std::cout << "eheh :: ->>>>>>> " << tmp << std::endl;
			if (cl->read_len == 0)
			{
				try
				{
					cl->read_len = std::stoi(tmp, 0, 16);
				}
				catch (std::exception &x)
				{
					std::cerr << x.what() << std::endl;
				}
				if (cl->read_len == 0)
				{
					cl->flag = 2;
					cl->read_status = 1;
					break;
				}
			}
			else if (tmp.size() + cl->readed < cl->read_len)
			{
				myfile1 << tmp;
				myfile1 << "\n";
				cl->readed += tmp.size() + 1;
			}
			else if (tmp.size() + cl->readed >= cl->read_len)
			{
				tmp = tmp + "\n";
				myfile1 << tmp;
				cl->readed += tmp.size() - 2;
				cl->read_len = 0;
			}
		}
	}
	// while (getline(myfile, tmp))
	// {
	// 	std::cout << "This is handle body <-----> ..............." << tmp << std::endl;
	// 	if (cl->chunked == 0)
	// 	{
	// 		if (tmp.size() + cl->readed < cl->read_len)
	// 		{
	// 			myfile1 << tmp;
	// 			myfile1 << "\n";
	// 			cl->readed += tmp.size() + 1;
	// 		}
	// 		else if (tmp.size() + cl->readed >= cl->read_len)
	// 		{
	// 			myfile1 << tmp;
	// 			cl->readed += tmp.size();
	// 			cl->flag = 2;
	// 			cl->read_status = 1;
	// 		}
	// 		else
	// 		{
	// 			myfile1.write(tmp.c_str(), cl->read_len - cl->readed);
	// 			cl->readed += cl->read_len - cl->readed;
	// 			cl->flag = 2;
	// 			cl->read_status = 1;
	// 		}
	// 	}
	// 	else
	// 	{
	// 		if (cl->read_len == 0)
	// 		{
	// 			try
	// 			{
	// 				cl->read_len = std::stoi(tmp, 0, 16);
	// 			}
	// 			catch (std::exception &x)
	// 			{
	// 				std::cerr << x.what() << std::endl;
	// 			}
	// 			if (cl->read_len == 0)
	// 			{
	// 				cl->flag = 2;
	// 				cl->read_status = 1;
	// 				break;
	// 			}
	// 		}
	// 		else if (tmp.size() + cl->readed < cl->read_len)
	// 		{
	// 			myfile1 << tmp;
	// 			myfile1 << "\n";
	// 			cl->readed += tmp.size() + 1;
	// 		}
	// 		else if (tmp.size() + cl->readed >= cl->read_len)
	// 		{
	// 			tmp = tmp + "\n";
	// 			myfile1 << tmp;
	// 			cl->readed += tmp.size() - 1;
	// 		}
	// 		else
	// 		{
	// 			myfile1 << tmp;
	// 			cl->readed += tmp.size();
	// 		}
	// 		if (cl->readed >= cl->read_len)
	// 		{
	// 			cl->read_len = 0;
	// 			cl->readed = 0;
	// 		}
	// 	}
	// }
	// if (cl->read_status == 0 && cl->flag == 1)
	// {
	// 	cl->read_status = 1;
	// 	--cl->readed;
	// }
	std::cout << "[" << cl->readed << "] <----->  this is cl->readed from c-length ===  " << cl->read_len << "\n"
			  << "---------read status--- " << cl->read_status << " ------ flag----" << cl->flag << std::endl;
}

request::request(client *cl, std::string s)
{
	std::ofstream myfile1;
	std::ostringstream ss2;
	std::string tmp;
	std::string tmp1;

	int i = 0;
	ss2 << "/tmp/body_" << cl->client_fd;
	myfile1.open(ss2.str());
	if (myfile1.fail())
		exit(0);
	size_t endOfHeadres = s.find("\r\n\r\n");
	if (endOfHeadres == std::string::npos)
	{
		std::cout << "error\n";
	}
	// int p = 0;
	tmp1 = s.substr(0, endOfHeadres);
	s.erase(0, s.find("\r\n\r\n") + 4);
	std::cout << "[" << s << "]" << std::endl;
	std::stringstream ss(tmp1);
	while (getline(ss, tmp))
	{
		std::istringstream iss(tmp);
		if (i == 0 && cl->flag == 0)
		{
			iss >> method;
			iss >> absoluteURI;
			iss >> http_version;
			// std::cout << "method : " << method << " abslu URI: " << absoluteURI << " http Version: " << http_version << std::endl;
		}
		else if (tmp.find(":") != std::string::npos && cl->flag == 0)
		{
			std::string tmp2;
			std::string tmp3;
			iss >> tmp2;
			iss >> tmp3;
			std::pair<std::string, std::string> pr;
			pr.first = tmp2;
			pr.second = tmp3;
			data.insert(pr);
			// std::cout << pr.first << " : " << pr.second << std::endl;
		}
		i++;
	}
	cl->flag = 1;
	end_of_headers(this, cl);
	this->handle_body(cl, s);
}
// if (cl->read_status == 0 && cl->flag == 1)
// {
// 	cl->read_status = 1;
// 	--cl->readed;
// }
// std::cout << "[" << cl->readed << "] <----->  this is cl->readed" << std::endl;
// std::cout << ">>>.<<<" << cl->flag << "   status  ->>" << cl->read_status << std::endl;

request::request() {}