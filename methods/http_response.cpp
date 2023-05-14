#include "http_response.hpp"

response::response(request* _req, server _config)
{
	this->req = _req;
	this->config = new server(_config);
    std::cout << "----+------" << config->location[1].location_name << std::endl;

	// std::cout << config->root << std::endl;
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
	std::cout << "Generateindex.html\n";
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
		content_lenght = htmlfile.str().size();
		status = status_code(200);
		content_type = "text/html";
		return generateResponseHeader() + htmlfile.str();
	}
	//!generate header
	return status_code(404);
}

std::string response::getfile()
{
	std::cout << "file: " << req->absoluteURI << std::endl;
	std::ifstream file(req->absoluteURI);
	if (!file.is_open()) {
		std::cout <<"here\n";
		std::cerr << "Failed to open file: " << req->absoluteURI << '\n';
		status = status_code(404);
		content_type = "text/html";
		content_lenght = status.size();
		return generateResponseHeader() + status;
	}
	std::ostringstream file_content;
	file_content << file.rdbuf();
	file.close();
	status = status_code(200);
	content_lenght = file_content.str().size();
	content_type = contentType();
	return generateResponseHeader() + file_content.str();
}

std::string response::getfolder()
{
	if(req->absoluteURI.back() != '/')
	{
		content_type = "text/html";
		status = status_code(301);
		content_lenght = status.size();
		return generateResponseHeader() + status;
	}
	if (!config->index.empty())
	{
		req->absoluteURI +=  config->index[0]; 
		return getfile();
	}
	else if(config->autoindex == "ON")
		return createIndexHtml();
	else if(config->autoindex == "OFF" )
	{
		status = status_code(301);
		content_type = "text/html";
		content_lenght = status.size();
		return generateResponseHeader() + status;
	}
	//!generate response
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
		return "text/html";
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
	std::string header = "HTTP/1.1 200 OK\r\n";
	header += "content-lype: " + contentType() + "\r\n";
	header += "content-lenght: " + std::to_string(this->content_lenght) + "\r\n";
	header += "server: nginxa\r\n";
	header += "cache-control: max-age=3600\r\n";
	header += "date: " + get_date() + "\r\n\r\n";
	// header += "connection: close\r\n\r\n";
	return header;
}

std::string	response::matchLocation()
{
	int index = -1;
	std::string location;
	std::cout << "absolutURI " + req->absoluteURI << std::endl; 
	for(size_t i = 0; i < config->location.size(); i++)
	{
		std::cout << "config_location" + std::to_string(i) + " " + config->location[i].location_name << std::endl;
		if(req->absoluteURI.find(config->location[i].location_name) == 0)
		{
			std::cout << "find\n";
			if(config->location[i].location_name.size() > location.size())
			{
				std::cout << "locationNow: " << location << std::endl;
				location = config->location[i].location_name;
				index = i;
			}
		}
	}
	std::cout << "location:::::: " << location << std::endl;
	if(!location.empty())
		return config->location[index].root + req->absoluteURI;
	else if(!config->root.empty())
		return config->root + req->absoluteURI;
	return "";

}

std::string   response::get_response()
{
	// std::cout << "----++------" << config->location[1].location_name << std::endl;
	std::cout << "root------ " << this->config->root << std::endl;
	std::cout << "URI1------- " << req->absoluteURI << std::endl;
	//!generate header
	if(!config->chunked_transfer_encoding.empty() && config->chunked_transfer_encoding != "chunked")
		return status_code(501);
	if(req->absoluteURI.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%") != std::string::npos)
		return status_code(400);
	else if(req->absoluteURI.size() > 2048)
		return status_code(414);
	// req->absoluteURI = config->root + req->absoluteURI;
	// req->absoluteURI = "/Users/omar/Desktop" + req->absoluteURI;
	// std::cout << "URI2------- " << req->absoluteURI << std::endl;
	req->absoluteURI = matchLocation();
	if(req->absoluteURI.empty())
		return status_code(404);
	std::string pathtype = checkPathType();
	if(pathtype == "FILE")
		return getfile();
	else if(pathtype == "FOLDER")
		return getfolder();
	std::cout << "here\n";
	content_type = "text/html";
	status = status_code(404);
	content_lenght = status.size();
	return generateResponseHeader() + status;
}

std::string	response::status_code(int status_code)
{
	if(status_code == 200)
		return "HTTP/1.1 200 Ok\r\n";
	if(status_code == 301)
		return "HTTP/1.1 301 Moved Permanently\r\n";
	else if(status_code == 400)
		return "HTTP/1.1 400 Bad Request\r\n";
	else if(status_code == 404)
		return "HTTP/1.1 404 Not Found\r\n";
	else if(status_code == 501)
		return "HTTP/1.1 501 Not Implemented\r\n";
	else if(status_code == 413)
		return "HTTP/1.1 413 Request Entity Too Large\r\n";
	else if(status_code == 414)
		return "HTTP/1.1 414 Request-URI Too Long\r\n";
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