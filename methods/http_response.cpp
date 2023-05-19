#include "http_response.hpp"

response::response(request* _req, server _config)
{
	this->req = _req;
	this->config = new server(_config);
	
	mimeTypeMap.insert(std::make_pair("aac", "audio/aac"));
	mimeTypeMap.insert(std::make_pair("abw", "application/x-abiword"));
	mimeTypeMap.insert(std::make_pair("arc", "application/octet-stream"));
	mimeTypeMap.insert(std::make_pair("avi", "video/x-msvideo"));
	mimeTypeMap.insert(std::make_pair("azw", "application/vnd.amazon.ebook"));
	mimeTypeMap.insert(std::make_pair("bin", "application/octet-stream"));
	mimeTypeMap.insert(std::make_pair("bmp", "image/bmp"));
	mimeTypeMap.insert(std::make_pair("bz", "application/x-bzip"));
	mimeTypeMap.insert(std::make_pair("bz2", "application/x-bzip2"));
	mimeTypeMap.insert(std::make_pair("csh", "application/x-csh"));
	mimeTypeMap.insert(std::make_pair("css", "text/css"));
	mimeTypeMap.insert(std::make_pair("csv", "text/csv"));
	mimeTypeMap.insert(std::make_pair("doc", "application/msword"));
	mimeTypeMap.insert(std::make_pair("docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document"));
	mimeTypeMap.insert(std::make_pair("eot", "application/vnd.ms-fontobject"));
	mimeTypeMap.insert(std::make_pair("epub", "application/epub+zip"));
	mimeTypeMap.insert(std::make_pair("gif", "image/gif"));
	mimeTypeMap.insert(std::make_pair("htm", "text/html"));
	mimeTypeMap.insert(std::make_pair("html", "text/html"));
	mimeTypeMap.insert(std::make_pair("ico", "image/x-icon"));
	mimeTypeMap.insert(std::make_pair("ics", "text/calendar"));
	mimeTypeMap.insert(std::make_pair("jar", "application/java-archive"));
	mimeTypeMap.insert(std::make_pair("jpeg", "image/jpeg"));
	mimeTypeMap.insert(std::make_pair("jpg", "image/jpeg"));
	mimeTypeMap.insert(std::make_pair("js", "application/javascript"));
	mimeTypeMap.insert(std::make_pair("json", "application/json"));
	mimeTypeMap.insert(std::make_pair("mid", "audio/midi"));
	mimeTypeMap.insert(std::make_pair("midi", "audio/midi"));
	mimeTypeMap.insert(std::make_pair("mpeg", "video/mpeg"));
	mimeTypeMap.insert(std::make_pair("mpkg", "application/vnd.apple.installer+xml"));
	mimeTypeMap.insert(std::make_pair("odp", "application/vnd.oasis.opendocument.presentation"));
	mimeTypeMap.insert(std::make_pair("ods", "application/vnd.oasis.opendocument.spreadsheet"));
	mimeTypeMap.insert(std::make_pair("odt", "application/vnd.oasis.opendocument.text"));
	mimeTypeMap.insert(std::make_pair("oga", "audio/ogg"));
	mimeTypeMap.insert(std::make_pair("ogv", "video/ogg"));
	mimeTypeMap.insert(std::make_pair("ogx", "application/ogg"));
	mimeTypeMap.insert(std::make_pair("otf", "font/otf"));
	mimeTypeMap.insert(std::make_pair("png", "image/png"));
	mimeTypeMap.insert(std::make_pair("pdf", "application/pdf"));
	mimeTypeMap.insert(std::make_pair("ppt", "application/vnd.ms-powerpoint"));
	mimeTypeMap.insert(std::make_pair("pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation"));
	mimeTypeMap.insert(std::make_pair("rar", "application/x-rar-compressed"));
	mimeTypeMap.insert(std::make_pair("rtf", "application/rtf"));
	mimeTypeMap.insert(std::make_pair("sh", "application/x-sh"));
	mimeTypeMap.insert(std::make_pair("svg", "image/svg+xml"));
	mimeTypeMap.insert(std::make_pair("swf", "application/x-shockwave-flash"));
	mimeTypeMap.insert(std::make_pair("tar", "application/x-tar"));
	mimeTypeMap.insert(std::make_pair("tif", "image/tiff"));
	mimeTypeMap.insert(std::make_pair("tiff", "image/tiff"));
	mimeTypeMap.insert(std::make_pair("ts", "application/typescript"));
	mimeTypeMap.insert(std::make_pair("ttf", "font/ttf"));
	mimeTypeMap.insert(std::make_pair("vsd", "application/vnd.visio"));
	mimeTypeMap.insert(std::make_pair("wav", "audio/x-wav"));
	mimeTypeMap.insert(std::make_pair("weba", "audio/webm"));
	mimeTypeMap.insert(std::make_pair("webm", "video/webm"));
	mimeTypeMap.insert(std::make_pair("webp", "image/webp"));
	mimeTypeMap.insert(std::make_pair("woff", "font/woff"));
	mimeTypeMap.insert(std::make_pair("woff2", "font/woff2"));
	mimeTypeMap.insert(std::make_pair("xhtml", "application/xhtml+xml"));
	mimeTypeMap.insert(std::make_pair("xls", "application/vnd.ms-excel"));
	mimeTypeMap.insert(std::make_pair("xlsx", "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"));
	mimeTypeMap.insert(std::make_pair("xml", "application/xml"));
	mimeTypeMap.insert(std::make_pair("xul", "application/vnd.mozilla.xul+xml"));
	mimeTypeMap.insert(std::make_pair("zip", "application/zip"));
	mimeTypeMap.insert(std::make_pair("3gp", "video/3gpp audio/3gpp"));
	mimeTypeMap.insert(std::make_pair("3g2", "video/3gpp2 audio/3gpp2"));
	mimeTypeMap.insert(std::make_pair("7z", "application/x-7z-compressed"));
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
		return errorPage(404);
	}
	std::ostringstream file_content;
	file_content << file.rdbuf();
	file.close();
	return generateResponseHeader(contentType(), std::to_string(file_content.str().size()), 200) + file_content.str();
}

std::string response::getfolder()
{
	if(req->absoluteURI.back() != '/')
	{
		req->absoluteURI.push_back('/');
		// status = 301;
	}
	if (!config->location[indexLocation].index.empty())
	{
		std::string pathfile;
		for(size_t i = 0; i < config->location[indexLocation].index.size(); i++)
		{
			pathfile = req->absoluteURI +  config->location[indexLocation].index[i]; 
			std::ifstream file(pathfile);
			if(file.is_open())
			{
				req->absoluteURI = pathfile;
				return getfile();
			}
		}
		return errorPage(404);
	}
	else if(config->location[indexLocation].autoindex == "ON")
		return createIndexHtml();
	else if(config->location[indexLocation].autoindex == "OFF" )
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
		return "";
	}
	return config->location[indexLocation].root + req->absoluteURI.substr(location.size());
}

bool response::methode_allowded(std::string methode)
{
	std::cout << "indexlocation-------------------:" << indexLocation << std::endl;
	if(config->location[indexLocation].methods.empty())
		return false;
	for(size_t i = 0; i < config->location[indexLocation].methods.size(); i++)
			if(config->location[indexLocation].methods[i] == methode)
				return true;
	return false;
}

std::string   response::get_response()
{
	// if(!req->chunked_transfer_encoding.empty() && req->chunked_transfer_encoding != "chunked")
		// return errorPage(501);
	// if(req->method == "POST" && (req->chunked_transfer_encoding.empty() || req->content-lenght.empty()))
		// return errorPage(400);
	std::cout << "URI1: " << req->absoluteURI << std::endl;
	if(req->absoluteURI.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%") != std::string::npos)
		return errorPage(400);
	else if(req->absoluteURI.size() > 2048)
		return errorPage(414);
	// else if(req->body.size() > std::stoi(config->client_max_body_size))
		// return errorPage(413);
	req->absoluteURI = matchLocation();
	if(req->absoluteURI.empty())
		return errorPage(404);
	std::cout << "URI2: " << req->absoluteURI << std::endl;
	if(req->method == "GET")
	{
		if(!methode_allowded("GET"))
			return errorPage(405);
		std::string pathtype = checkPathType();
		if(pathtype == "FILE")
			return getfile();
		else if(pathtype == "FOLDER")
			return getfolder();
		return errorPage(404);
	}
	else if(req->method == "POST")
	{
		// ! is_POST_method_allowded_in_location
		//  if(!config->location[indexLocation].upload.empty())
    	// {
        	// uplaod the Post Request Body
        	// return errorPage(201);
    	// }
    	// get_requested_resource()
    	// std::string pathtype = checkPathType();
    	// if(pathtype == "NOT FOUND")
    	    // return errorPage(404);
    	// if(pathtype == "FILE")
    	// {
    	    // if_location_has_cgi()
    	    // {
    	        // run cgi on requested file with POST REQUEST_METHOD
    	        // Returtn Code Depend on cgi
    	    // }
    	    // else 
    	        // return status_code(403);
    	// }
    	// else if(pathtype == "FOLDER")
    	// {
    	//         if(req->absoluteURI.back() != '/')
    	//         {
    	//                 // make a 301 redirection to request uri with “/” addeed at the end
    	//                 return errorPage(301);
    	//         }
    	//         if(!config->location[indexLocation].index.empty())
    	//         {
    	//             req->absoluteURI += config->location[indexLocation].index[0];
    	//             return getfile();
    	//         }
    	//         else
    	//             return errorPage(403);
    	// }

	}
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
	else if(status_code == 405)
		return "HTTP/1.1 405 Method Not Allowed\r\n";
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
	else if(code == 405)
		body = generateErrorPages(405);
	else if(code == 413)
		body = generateErrorPages(413);
	else if(code == 414)
		body = generateErrorPages(414);
	else if(code == 501)
		body = generateErrorPages(501);
	return generateResponseHeader("text/html", std::to_string(body.size()), code) + body;
}
