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
			std::cout << ">>>>>>>> this is it" << req->data.find("Content-Length:")->second << std::endl;
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
	std::string tmp1;
	std::string chunk;

	// std::cout << ">>>>>" << s << std::endl;
	// std::cout << "<<<<<<<<<<<<<<<<[this is handle body]>>>>>>>>>>>>>>>read status : " << cl->read_status << " and flag : " << cl->flag << " readed = " << cl->readed << " read_len = " << cl->read_len << std::endl;
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
		if (cl->read_len == 0)
		{
			char *tmp1 = strtok(const_cast<char *>(s.c_str()), "\r\n");
			std::string st(tmp1);
			try
			{
				cl->read_len = std::stol(st, 0, 16);
			}
			catch (std::exception &x)
			{
				std::cerr << x.what() << std::endl;
			}
			if (cl->read_len == 0 || s.size() == 0)
			{
				if (cl->read_len == 0)
				{
					cl->read_status = 1;
					cl->flag = 2;
				}
				return;
			}
			size_t save = cl->read_len;
			if (s.size() < cl->read_len)
				cl->read_len = s.size() - (s.find("\n") + 1);
			chunk = s.substr(s.find("\n") + 1, cl->read_len);
			myfile1 << chunk;
			// std::cout << ">>>>>>>>>>>>>>>>>>>" << chunk << std::endl;
			// std::cout << "chunk and read len:" << chunk.size() << " | " << cl->read_len << std::endl;
			// cl->readed = chunk.size();
			if (chunk.size() >= save)
				cl->read_len = 0;
			else if (chunk.size() < save)
			{
				cl->read_len = save;
				cl->readed = chunk.size();
			}
			myfile1.close();
			if (chunk.size() >= save)
			{
				handle_body(cl, s.substr(s.find("\n") + 3 + save, s.size()));
				return;
			}
		}
		else
		{
			size_t ab = cl->read_len - cl->readed;
			chunk = s.substr(0, ab);
			myfile1 << chunk;
			if (chunk.size() == ab)
				cl->read_len = 0;
			else
			{
				cl->readed += chunk.size();
			}
			myfile1.close();
			// std::cout << s << " | " << s.size() << " | " << ab << std::endl;
			if (ab < s.size())
				handle_body(cl, s.substr(ab + 2, s.size() - ab));
		}
		myfile1.close();
		// if (s.find("\r\n0\r\n\r\n") == std::string::npos)
		// {
		// 	if (s.find("\r\n\r\n") != std::string::npos)
		// 	{
		// 		std::cout << "alloha lloha dfklsdf >>>>>>>>>>>>>>>>>>>>" << std::endl;
		// 	}
		// 	myfile1 << s;
		// 	cl->readed += s.size();
		// 	if (cl->readed >= cl->read_len)
		// 	{
		// 		cl->readed = 0;
		// 	}
		// }
		// else
		// {
		// 	s.erase(s.size() - 7, s.size());
		// 	std::cout << "[" << s << "]" << std::endl;
		// 	myfile1 << s;
		// 	cl->readed += s.size();
		// 	if (cl->readed >= cl->read_len)
		// 	{
		// 		cl->read_status = 1;
		// 		cl->flag = 2;
		// 	}
		// }
	}
	// std::cout << "[" << cl->readed << "] <----->  this is cl->readed from c-length ===  " << cl->read_len << "\n"
	// 		  << "---------read status--- " << cl->read_status << " ------ flag----" << cl->flag << std::endl;
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
	tmp1 = s.substr(0, endOfHeadres);
	s = s.erase(0, s.find("\r\n\r\n") + 4);
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
	if (cl->read_status == 0)
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