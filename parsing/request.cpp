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
			cl->read_len = stol(req->data.find("Content-Length:")->second);
		}
		catch (std::exception &x)
		{
			std::cout << x.what() << std::endl;
		}
	}
	if (cl->read_len == 0 && cl->chunked == 0){
		cl->read_status = 1;
	}
}

void request::handle_body(client *cl, std::string s)
{
	std::ostringstream ss2;
	std::ofstream myfile1;
	std::string tmp;
	std::string tmp1;
	std::string chunk;

	ss2 << "/tmp/body_" << cl->client_fd;
	myfile1.open(ss2.str(), std::ofstream::app);
	if (cl->chunked == 0 && cl->read_len){
		myfile1 << s;
		cl->readed += s.size();
		if (cl->readed >= cl->read_len)
		{
			cl->read_status = 1;
			cl->flag = 2;
		}
		readed = cl->readed;
	}
	else
	{
		if (cl->read_len == cl->readed)
		{
			std::string st(s.substr(0, s.find("\r\n")));
			s = s.erase(0, s.find("\r\n") + 2);
			try{
				cl->read_len = std::stol(st, 0, 16);
			}
			catch (std::exception &x){
				std::cerr << x.what() << std::endl;
			}
			size_t save = cl->read_len;
			if (cl->read_len == 0 || s.size() == 0)
			{
				if (cl->read_len == 0)
				{
					cl->read_status = 1;
					cl->flag = 2;
				}
				return ;
			}
			if (s.size() < cl->read_len)
				cl->read_len = s.size();
			chunk = s.substr(0, cl->read_len);
			myfile1 << chunk;
			cl->readed += chunk.size();
			if (chunk.size() == save){
				readed += cl->readed;
				cl->read_len = 0;
				cl->readed = 0;
				if (cl->read_len == s.size() || cl->read_len == s.size() -2)
					return ;
				s = s.erase(0, chunk.size() + 2);
				myfile1.close();
				if(s.size() == 0)
					return ;
				handle_body(cl, s);
			}
			else if (save >= cl->readed)
			{
				if (save >= s.size())
					s.resize(0);
				cl->read_len = save;
				myfile1.close();
				return ;
			}
		}
		else if (cl->readed < cl->read_len)
		{
			size_t ab = cl->read_len - cl->readed;
			size_t save = ab;
			if (ab > s.size())
				ab = s.size();
			chunk = s.substr(0, ab);
			myfile1 << chunk;
			cl->readed = cl->readed + chunk.size();
			if (chunk.size() == save){
				readed += cl->readed;
				cl->read_len = 0;
				cl->readed = 0;
				if (ab == s.size() || ab + 2 == s.size())
					return ;
				s = s.erase(0, chunk.size() + 2);
				myfile1.close();
				handle_body(cl, s);
			}
			else if(chunk.size() <= save)
			{
				if (save >= s.size())
					s.resize(0);
				myfile1.close();
				return ;
			}
		}
	}
	myfile1.close();
	return ;
}

int check_header(const std::string &s)
{
	for (size_t i = 0; i < s.length(); i++)
	{
		if (!isalpha(s[i]) && s[i] != '-')
			return 0;
	}
	return 1;
}

request::request(client *cl, std::string s)
{
	std::ofstream myfile1;
	std::ostringstream ss2;
	std::string tmp;
	std::string tmp1;

	int i = 0;
	ss2 << "/tmp/body_" << cl->client_fd;
	this->readed = 0;
	myfile1.open(ss2.str());
	cl->client_body = ss2.str();
	if (myfile1.fail())
		exit(0);
	size_t endOfHeadres = s.find("\r\n\r\n");
	if (endOfHeadres == std::string::npos)
		std::cout << "error\n";
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
			if (absoluteURI.find("?") != std::string::npos){
				std::pair<std::string, std::string> p("Query-String:", absoluteURI.substr(absoluteURI.find("?"), absoluteURI.size()));
				data.insert(p);
				absoluteURI = absoluteURI.substr(0, absoluteURI.find("?"));
			}
			iss >> http_version;
		}
		else if (tmp.find(":") != std::string::npos && cl->flag == 0)
		{
			check_header(tmp.substr(0, tmp.find(":")));
			std::string tmp2;
			std::string tmp3;
			iss >> tmp2;
			iss >> tmp3;
			if (tmp2 == "Content-Type:" && tmp3 == "multipart/form-data;")
			{
				iss >> boundry;
				tmp3 += " " + boundry;
				boundry.erase(0, 9);
			}
			if (tmp2 == "Cookie:")
				tmp3 = tmp.substr(8);
			std::pair<std::string, std::string> pr;
			pr.first = tmp2;
			pr.second = tmp3;
			data.insert(pr);
		}
		i++;
	}
	cl->flag = 1;
	end_of_headers(this, cl);
	if (cl->read_len == 0 && cl->chunked == 0)
		cl->flag = 2;
	if (cl->read_status == 0)
		this->handle_body(cl, s);
	myfile1.close();
}

request::request() {}