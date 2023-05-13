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
			cl->read_len = stoi(req->data.find("Content-Length:")->second);
		}
		catch (std::exception &x)
		{
			std::cout << x.what() << std::endl;
		}
	}
	if (cl->read_len == 0 && cl->chunked == 0)
		cl->read_status = 1;
}

void request::handle_body(client *cl)
{
	std::ostringstream ss1;
	std::ifstream myfile;
	std::ostringstream ss2;
	std::ifstream myfile1;
	std::string tmp;

	ss1 << "/tmp/request_" << cl->client_fd;
	ss2 << "/tmp/body_" << cl->client_fd;
	myfile.open(ss1.str());
	if (myfile.fail())
		exit(0);
	// myfile1.open(ss2.str(), std::ifstream::app);
}

request::request(client *cl)
{
	std::ifstream myfile;
	std::ofstream myfile1;
	std::ostringstream ss1;
	std::ostringstream ss2;
	std::string tmp;
	int i = 0;
	ss1 << "/tmp/request_" << cl->client_fd;
	ss2 << "/tmp/body_" << cl->client_fd;
	myfile.open(ss1.str());
	myfile1.open(ss2.str());
	if (myfile.fail())
		exit(0);
	while (getline(myfile, tmp))
	{
		std::istringstream iss(tmp);
		if (tmp == "\r")
		{
			if (cl->flag == 0)
			{
				cl->flag = 1;
				end_of_headers(this, cl);
			}
			else
			{
				cl->flag = 2;
				cl->read_status = 1;
			}
		}
		else if (i == 0 && cl->flag == 0)
		{
			iss >> method;
			iss >> absoluteURI;
			iss >> http_version;
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
			std::cout << tmp << std::endl;
		}
		else
		{
			if (cl->chunked == 0)
			{
				if (tmp.size() + cl->readed < cl->read_len)
				{
					myfile1 << tmp;
					myfile1 << "\n";
					cl->readed += tmp.size() + 1;
				}
				else if (tmp.size() + cl->readed == cl->read_len)
				{
					myfile1 << tmp;
					cl->readed += tmp.size();
					cl->flag = 2;
					cl->read_status = 1;
				}
				else
				{
					myfile1.write(tmp.c_str(), cl->read_len - cl->readed);
					cl->readed += cl->read_len - cl->readed;
					cl->flag = 2;
					cl->read_status = 1;
				}
			}
		}
		i++;
	}
	if (cl->read_status == 0 && cl->flag == 1)
	{
		cl->read_status = 1;
		--cl->readed;
	}
	std::cout << "[" << cl->readed << "] <----->  this is cl->readed" << std::endl;
	std::cout << ">>>.<<<" << cl->flag << "   status  ->>" << cl->read_status << std::endl;
}
request::request() {}