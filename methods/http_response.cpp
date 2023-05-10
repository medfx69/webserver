#include "http_response.hpp"

response::response()
{
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


std::string    response::checkPathType(Parsed* data)
{
	struct stat st;
	if(stat(data->req->absoluteURI.c_str(), &st) != 0)
		return "NOT FOUND";
	else if(S_ISDIR(st.st_mode))
		return "FOLDER";
	else
		return "FILE";
}

std::string response::createIndexHtml(std::string pathdir)
{
	DIR* dir = opendir(pathdir.c_str());
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
	return "HTTP/1.1 404 Not Found\r\n\r\n";
}

std::string response::getfile(std::string pathfile)
{
	std::ifstream file(pathfile);
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << pathfile << '\n';
		return "HTTP/1.1 404 Not Found\r\n\r\n";
	}
	std::ostringstream file_content;
	file_content << file.rdbuf();
	file.close();
	this->content_lenght = file_content.str().size();
	return generateResponseHeader(pathfile) + file_content.str();
}

std::string response::getfolder(Parsed* data, server config)
{
	if(data->req->absoluteURI.back() != '/')
		return "HTTP/1.1 301 Moved Permanently\r\n\r\n";
	if (!config.index.empty())
		return getfile(data->req->absoluteURI + "/" + config.index[0]);
	else if(config.autoindex == "OFF")
		return "HTTP/1.1 301 Moved Permanently\r\n\r\n";
	else if(config.autoindex == "ON")
		return createIndexHtml(data->req->absoluteURI);
	return NULL;
}

std::string response::getFileExtension(const std::string& filePath)
{
	size_t dotPos = filePath.find_last_of('.');
	if (dotPos == std::string::npos)
		return "";
	return filePath.substr(dotPos + 1);
}

std::string response::contentType(const std::string& filePath)
{
	std::string extension = getFileExtension(filePath);
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

std::string response::generateResponseHeader(const std::string& filePath)
{
	std::string header = "HTTP/1.1 200 OK\r\n\r\n";
	header += "Content-Type: " + contentType(filePath) + "\r\n";
	header += "Content-Lenght: " + std::to_string(this->content_lenght) + "\r\n";
	header += "Server: nginxa\r\n";
	header += "Cache-Control: max-age=3600\r\n";
	header += "Date: " + get_date();
	header += "Connection: close\r\n\r\n";
	return header;
}

void	matchLocation(Parsed* data, server& config)
{
	config.location[0].location_name += data->req->absoluteURI;
}


std::string   response::get_response(Parsed* data, server config)
{
	matchLocation(data, config);
	std::string pathtype = checkPathType(data);
	if(pathtype == "FILE")
		return getfile(data->req->absoluteURI);
	else if(pathtype == "FOLDER")
		return getfolder(data, config);
	return "HTTP/1.1 404 Not Found\r\n\r\n";
}

std::string	responseError()
{
	return "HTTP/1.1 404 Not Found\r\n\r\n";
	return "HTTP/1.1 301 Moved Permanently\r\n\r\n";
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
