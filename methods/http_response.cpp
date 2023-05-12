#include "http_response.hpp"

response::response(request* _req, server _config)
{
	this->req = _req;
	this->config = _config;
	std::cout << config.root << std::endl;
	mimeTypeMap.insert(std::make_pair("html", "text/html"));
    mimeTypeMap.insert(std::make_pair("htm", "text/html"));
    mimeTypeMap.insert(std::make_pair("css", "text/css"));
    mimeTypeMap.insert(std::make_pair("js", "application/javascript"));
    mimeTypeMap.insert(std::make_pair("jpg", "image/jpeg"));
    mimeTypeMap.insert(std::make_pair("jpeg", "image/jpeg"));
    mimeTypeMap.insert(std::make_pair("png", "image/png"));
    mimeTypeMap.insert(std::make_pair("gif", "image/gif"));
    mimeTypeMap.insert(std::make_pair("pdf", "application/pdf"));
}


std::string    response::checkPathType()
{
	struct stat st;
	if(stat(req->absoluteURI.c_str(), &st) != 0)
		return "NOT FOUND";
	else if(S_ISDIR(st.st_mode))
		return "FOLDER";
	else
		return "FILE";
}

std::string response::createIndexHtml()
{
	DIR* dir = opendir(req->absoluteURI.c_str());
	if(dir != NULL)
	{
		// std::ofstream htmlfile("index.html");
		std::ostringstream htmlfile;
		htmlfile << "<!DOCTYPE html>\n<html lang=\"en\">\n\t\t<body>\n";
		struct dirent* entry;
		while((entry = readdir(dir)) != NULL)
		{
			std::string fd = entry->d_name;
			if(fd == "." || fd == "..")
				;
			else
				htmlfile << "\t\t\t<a href='" << "./" << fd << "'>" << fd << "</a><br /><br />" << std::endl;
		}
		closedir(dir);
		htmlfile << "\t\t</body>\n</html>";
		return htmlfile.str();
	}
	return status_code(404);
}

std::string response::getfile()
{
	std::ifstream file(req->absoluteURI);
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << req->absoluteURI << '\n';
		return status_code(404);
	}
	std::ostringstream file_content;
	file_content << file.rdbuf();
	file.close();
	this->content_lenght = file_content.str().size();
	return generateResponseHeader() + file_content.str();
	return file_content.str();
}

std::string response::getfolder()
{
	if(req->absoluteURI.back() != '/')
		return status_code(301);
	if (!config.index.empty())
	{
		req->absoluteURI += "/" + config.index[0];
		return getfile();
	}
	else if(config.autoindex == "OFF")
		return status_code(301);
	else if(config.autoindex == "ON")
		return createIndexHtml();
	return NULL;
}

std::string response::getFileExtension()
{
	size_t dotPos = req->absoluteURI.find_last_of('.');
	if (dotPos == std::string::npos)
		return "";
	return req->absoluteURI.substr(dotPos + 1);
}

std::string response::contentType()
{
	std::string extension = getFileExtension();
	if(mimeTypeMap.count(extension) > 0)
		return mimeTypeMap.at(extension);
	else
		return "application/octet-stream";
}

std::string	response::get_date()
{
	time_t     now = time(0);
	struct tm *newtime = localtime(&now);
	char buf[80];
	strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", newtime);
	std::string date_str(buf);
	return date_str;
}

std::string response::generateResponseHeader()
{
	std::string header = "HTTP/1.1 200 OK\r\n\r\n";
	header += "Content-Type: " + contentType() + "\r\n";
	header += "Content-Lenght: " + std::to_string(this->content_lenght) + "\r\n";
	header += "Server: nginxa\r\n";
	header += "Cache-Control: max-age=3600\r\n";
	header += "Date: " + get_date();
	header += "Connection: close\r\n\r\n";
	return header;
}

std::string	response::matchLocation()
{
	int index = -1;
	std::string location;
	for(size_t i = 0; i < config.location.size(); i++)
	{
		if(req->absoluteURI.find(config.location[i].location_name) == 0)
		{
			if(config.location[i].location_name.size() > location.size())
			{
				location = config.location[i].location_name;
				index = i;
			}
		}
	}
	if(!location.empty())
		return config.location[index].root + req->absoluteURI;
	else if(!config.root.empty())
		return config.root + req->absoluteURI;
	return "";

}

std::string   response::get_response()
{
	std::cout << "root------ " << this->config.root << std::endl;
	std::cout << "URI1------- " << req->absoluteURI << std::endl;
	// exit(0);
	// matchLocation(req, config);
	// std::cout << "config+++++++++ " << req->absoluteURI << std::endl;
	// if(!config.chunked_transfer_encoding.empty() && config.chunked_transfer_encoding != "chunked")
	// 	return status_code(501);
	if(req->absoluteURI.find_first_of(":?#[]@!$&'()*+,;=") != std::string::npos)
		return status_code(400);
	else if(req->absoluteURI.size() > 2048)
		return status_code(414);
	// req->absoluteURI = config.root + req->absoluteURI;
	req->absoluteURI = "/Users/oel-berh/Desktop" + req->absoluteURI;
	// req->absoluteURI = matchLocation();
	std::cout << "URI2------- " << req->absoluteURI << std::endl;
	if(req->absoluteURI.empty())
		return status_code(404);
	std::string pathtype = checkPathType();
	if(pathtype == "FILE")
		return getfile();
	else if(pathtype == "FOLDER")
		return getfolder();
	return status_code(404);
}

std::string	response::status_code(int status_code)
{
	if(status_code == 301)
		return "HTTP/1.1 301 Moved Permanently\r\n\r\n";
	else if(status_code == 400)
		return "HTTP/1.1 400 Bad Request\r\n\r\n";
	else if(status_code == 404)
		return "HTTP/1.1 404 Not Found\r\n\r\n";
	else if(status_code == 501)
		return "HTTP/1.1 501 Not Implemented\r\n\r\n";
	else if(status_code == 413)
		return "HTTP/1.1 413 Request Entity Too Large\r\n\r\n";
	else if(status_code == 414)
		return "HTTP/1.1 414 Request-URI Too Long\r\n\r\n";
	return NULL;
}

// int main()
// {
// 	std::string pathdir = "/Users/omar/Desktop/leetcode";
// 	DIR* dir = opendir(pathdir.c_str());
// 	if(dir != NULL)
// 	{
// 		std::ofstream htmlfile("index.html");
// 		// std::ostringstream htmlfile;
// 		htmlfile << "<!DOCTYPE html>\n<html lang=\"en\">\n\t\t<body>\n";
// 		struct dirent* entry;
// 		while((entry = readdir(dir)) != NULL)
// 		{
// 			std::string fd = entry->d_name;
// 			if(fd == "." || fd == "..")
// 				;
// 			else
// 				htmlfile << "\t\t\t<a href='" << "./" << fd << "'>" << fd << "</a><br /><br />" << std::endl;
// 		}
// 		closedir(dir);
// 		htmlfile << "\t\t</body>\n</html>";
// 	}
// 	return 0;
// 	// return "HTTP/1.1 404 Not Found\r\n\r\n";
// }
