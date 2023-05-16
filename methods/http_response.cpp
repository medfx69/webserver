#include "http_response.hpp"

response::response(request* _req, server _config)
{
	this->req = _req;
	this->config = new server(_config);
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
		return generateResponseHeader("text/html", std::to_string(status.size()), 200) + htmlfile.str();
	}
	std::string body = generateErrorPages(404);
	return generateResponseHeader("text/html", std::to_string(body.size()), 404) + body;
}

std::string response::getfile()
{
	std::cout << "file: " << req->absoluteURI << std::endl;
	std::ifstream file(req->absoluteURI);
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << req->absoluteURI << '\n';
		std::string body = generateErrorPages(404);
		return generateResponseHeader("text/html", std::to_string(body.size()), 404) + body;
	}
	std::ostringstream file_content;
	file_content << file.rdbuf();
	file.close();
	return generateResponseHeader(contentType(), std::to_string(file_content.str().size()), 200) + file_content.str();
}

std::string response::getfolder()
{
	if(req->absoluteURI.back() != '/')
		return errorPage(301);
	if (!config->index.empty())
	{
		req->absoluteURI +=  config->index[0]; 
		return getfile();
	}
	else if(config->autoindex == "ON")
		return createIndexHtml();
	else if(config->autoindex == "OFF" )
		return errorPage(301);
	return errorPage(404);
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
		return "text/plain";
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

std::string response::generateResponseHeader(std::string content_type, std::string content_lenght, int code)
{
	std::string header = status_code(code);
	header += "content-type: " + content_type + "\r\n";
	header += "content-lenght: " + content_lenght + "\r\n";
	header += "server: nginxa\r\n";
	header += "cache-control: max-age=3600\r\n";
	header += "date: " + get_date() + "\r\n\r\n";
	// header += "connection: close\r\n\r\n";
	return header;
}

std::string	response::matchLocation()
{
	indexLocation = -1;
	std::string location;
	std::cout << "absolutURI " + req->absoluteURI << std::endl; 
	for(size_t i = 0; i < config->location.size(); i++)
	{
		if(req->absoluteURI.find(config->location[i].location_name) == 0)
		{
			if(config->location[i].location_name.size() > location.size())
			{
				location = config->location[i].location_name;
				indexLocation = i;
			}
		}
	}
	if(indexLocation == -1 || config->location[indexLocation].root.empty())
	{
		std::cout << "Bad URL\n";
		return errorPage(404);
	}
	return config->location[indexLocation].root + req->absoluteURI.substr(location.size());
}

std::string   response::get_response()
{
	std::cout << "URI1: " << req->absoluteURI << std::endl;
	if(!config->chunked_transfer_encoding.empty() && config->chunked_transfer_encoding != "chunked")
		return errorPage(501);
	if(req->absoluteURI.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%") != std::string::npos)
		return errorPage(400);
	else if(req->absoluteURI.size() > 2048)
		return errorPage(414);
	req->absoluteURI = matchLocation();
	std::cout << "URI2: " << req->absoluteURI << std::endl;
	if(req->absoluteURI.empty())
		return errorPage(404);
	std::string pathtype = checkPathType();
	if(pathtype == "FILE")
		return getfile();
	else if(pathtype == "FOLDER")
		return getfolder();
	return errorPage(404);
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
	else if(status_code == 413)
		return "HTTP/1.1 413 Request Entity Too Large\r\n";
	else if(status_code == 414)
		return "HTTP/1.1 414 Request-URI Too Long\r\n";
	else if(status_code == 501)
		return "HTTP/1.1 501 Not Implemented\r\n";
	return NULL;
}

std::string response::generateErrorPages(int code)
{
	std::ifstream file;
	file.open("/Users/omar/Desktop/webserver/error_pages/" + std::to_string(code) + ".html");
	std::ostringstream content;
	content << file.rdbuf();
	return content.str();
}

std::string	response::errorPage(int code)
{
	std::string body;
	if(code == 301)
		body = generateErrorPages(301);
	else if(code == 400)
		body = generateErrorPages(400);
	else if(code == 404)
		body = generateErrorPages(404);
	else if(code == 413)
		body = generateErrorPages(413);
	else if(code == 414)
		body = generateErrorPages(414);
	else if(code == 501)
		body = generateErrorPages(501);
	return generateResponseHeader("text/html", std::to_string(body.size()), code) + body;
}