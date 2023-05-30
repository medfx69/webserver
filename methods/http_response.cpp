#include "http_response.hpp"

response::response(request* _req, server _config)
{
	indexLocation = -1;
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
	mimeTypeMap.insert(std::make_pair("mp4", "video/mp4"));
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

	mimeTypeMap2.insert(std::make_pair("audio/aac", "aac"));
	mimeTypeMap2.insert(std::make_pair("application/x-abiword", "abw"));
	mimeTypeMap2.insert(std::make_pair("application/octet-stream", "arc"));
	mimeTypeMap2.insert(std::make_pair("video/x-msvideo", "avi"));
	mimeTypeMap2.insert(std::make_pair("application/vnd.amazon.ebook", "azw"));
	mimeTypeMap2.insert(std::make_pair("application/octet-stream", "bin"));
	mimeTypeMap2.insert(std::make_pair("image/bmp", "bmp"));
	mimeTypeMap2.insert(std::make_pair("application/x-bzip", "bz"));
	mimeTypeMap2.insert(std::make_pair("application/x-bzip2", "bz2"));
	mimeTypeMap2.insert(std::make_pair("application/x-csh", "csh"));
	mimeTypeMap2.insert(std::make_pair("text/css", "css"));
	mimeTypeMap2.insert(std::make_pair("text/csv", "csv"));
	mimeTypeMap2.insert(std::make_pair("application/msword", "doc"));
	mimeTypeMap2.insert(std::make_pair("application/vnd.openxmlformats-officedocument.wordprocessingml.document", "docx"));
	mimeTypeMap2.insert(std::make_pair("application/vnd.ms-fontobject", "eot"));
	mimeTypeMap2.insert(std::make_pair("application/epub+zip", "epub"));
	mimeTypeMap2.insert(std::make_pair("image/gif", "gif"));
	mimeTypeMap2.insert(std::make_pair("text/html", "htm"));
	mimeTypeMap2.insert(std::make_pair("text/html", "html"));
	mimeTypeMap2.insert(std::make_pair("image/x-icon", "ico"));
	mimeTypeMap2.insert(std::make_pair("text/calendar", "ics"));
	mimeTypeMap2.insert(std::make_pair("application/java-archive", "jar"));
	mimeTypeMap2.insert(std::make_pair("image/jpeg", "jpeg"));
	mimeTypeMap2.insert(std::make_pair("image/jpeg", "jpg"));
	mimeTypeMap2.insert(std::make_pair("application/javascript", "js"));
	mimeTypeMap2.insert(std::make_pair("application/json", "json"));
	mimeTypeMap2.insert(std::make_pair("audio/midi", "mid"));
	mimeTypeMap2.insert(std::make_pair("audio/midi", "midi"));
	mimeTypeMap2.insert(std::make_pair("video/mpeg", "mpeg"));
	mimeTypeMap2.insert(std::make_pair("application/vnd.apple.installer+xml", "mpkg"));
	mimeTypeMap2.insert(std::make_pair("application/vnd.oasis.opendocument.presentation", "odp"));
	mimeTypeMap2.insert(std::make_pair("application/vnd.oasis.opendocument.spreadsheet", "ods"));
	mimeTypeMap2.insert(std::make_pair("application/vnd.oasis.opendocument.text", "odt"));
	mimeTypeMap2.insert(std::make_pair("audio/ogg", "oga"));
	mimeTypeMap2.insert(std::make_pair("video/ogg", "ogv"));
	mimeTypeMap2.insert(std::make_pair("application/ogg", "ogx"));
	mimeTypeMap2.insert(std::make_pair("video/mp4", "mp4"));
	mimeTypeMap2.insert(std::make_pair("font/otf", "otf"));
	mimeTypeMap2.insert(std::make_pair("image/png", "png"));
	mimeTypeMap2.insert(std::make_pair("application/pdf", "pdf"));
	mimeTypeMap2.insert(std::make_pair("application/vnd.ms-powerpoint", "ppt"));
	mimeTypeMap2.insert(std::make_pair("application/vnd.openxmlformats-officedocument.presentationml.presentation", "pptx"));
	mimeTypeMap2.insert(std::make_pair("application/x-rar-compressed", "rar"));
	mimeTypeMap2.insert(std::make_pair("application/rtf", "rtf"));
	mimeTypeMap2.insert(std::make_pair("application/x-sh", "sh"));
	mimeTypeMap2.insert(std::make_pair("image/svg+xml", "svg"));
	mimeTypeMap2.insert(std::make_pair("application/x-shockwave-flash", "swf"));
	mimeTypeMap2.insert(std::make_pair("application/x-tar", "tar"));
	mimeTypeMap2.insert(std::make_pair("image/tiff", "tif"));
	mimeTypeMap2.insert(std::make_pair("image/tiff", "tiff"));
	mimeTypeMap2.insert(std::make_pair("application/typescript", "ts"));
	mimeTypeMap2.insert(std::make_pair("font/ttf", "ttf"));
	mimeTypeMap2.insert(std::make_pair("application/vnd.visio", "vsd"));
	mimeTypeMap2.insert(std::make_pair("audio/x-wav", "wav"));
	mimeTypeMap2.insert(std::make_pair("audio/webm", "weba"));
	mimeTypeMap2.insert(std::make_pair("video/webm", "webm"));
	mimeTypeMap2.insert(std::make_pair("image/webp", "webp"));
	mimeTypeMap2.insert(std::make_pair("font/woff", "woff"));
	mimeTypeMap2.insert(std::make_pair("font/woff2", "woff2"));
	mimeTypeMap2.insert(std::make_pair("application/xhtml+xml", "xhtml"));
	mimeTypeMap2.insert(std::make_pair("application/vnd.ms-excel", "xls"));
	mimeTypeMap2.insert(std::make_pair("application/vnd.openxmlformats-officedocument.spreadsheetml.sheet", "xlsx"));
	mimeTypeMap2.insert(std::make_pair("application/xml", "xml"));
	mimeTypeMap2.insert(std::make_pair("application/vnd.mozilla.xul+xml", "xul"));
	mimeTypeMap2.insert(std::make_pair("application/zip", "zip"));
	mimeTypeMap2.insert(std::make_pair("video/3gpp audio/3gpp", "3gp"));
	mimeTypeMap2.insert(std::make_pair("video/3gpp2 audio/3gpp2", "3g2"));
	mimeTypeMap2.insert(std::make_pair("application/x-7z-compressed", "7z"));
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
	std::string header;
	if(status == "301")
		header = status_code(301);
	else
		header = status_code(code);
	header += "content-type: " + content_type + "\r\n";
	header += "content-lenght: " + content_lenght + "\r\n";
	header += "server: nginxa\r\n";
	header += "cache-control: max-age=3600\r\n";
	if(indexLocation != -1)
	{
		if(!config->location[indexLocation].redirection.empty())
			header += "location: " + config->location[indexLocation].redirection + "\r\n";
		else if(status == "301")
			header += "location: /" + req->absoluteURI.substr(config->location[indexLocation].root.size()) + "\r\n";
	}
	header += "date: " + get_date() + "\r\n\r\n";
	std::cout << "======================header======================\n"<<header;
	return header;
}

std::string response::cleanupURI(std::string& uri)
{
    std::string cleanedURI;
    std::string::const_iterator it = uri.begin();

    while (it != uri.end())
    {
        if (*it != '/')
            cleanedURI.push_back(*it);
        else if (cleanedURI.empty() || cleanedURI.back() != '/')
            cleanedURI.push_back(*it);
        ++it;
    }
    return cleanedURI;
}

std::string	response::matchLocation()
{
	indexLocation = -1;
	std::string location;
	for(size_t i = 0; i < config->location.size(); i++)
	{
		if(req->absoluteURI.find(config->location[i].location_name) == 0)
		{
			if(config->location[i].location_name != "/" && req->absoluteURI.size() > config->location[i].location_name.size() && req->absoluteURI[config->location[i].location_name.size()] != '/')
				;
			else if(config->location[i].location_name.size() > location.size())
			{
				location = config->location[i].location_name;
				indexLocation = i;
			}
		}
	}
	if(indexLocation == -1 || config->location[indexLocation].root.empty())
		req->absoluteURI = "";
	else if (req->absoluteURI.find(config->location[indexLocation].root) != std::string::npos)
		req->absoluteURI = req->absoluteURI;
	else
		req->absoluteURI = config->location[indexLocation].root + req->absoluteURI.substr(location.size());
	req->absoluteURI = cleanupURI(req->absoluteURI);
	std::cout << "URI=======================| " << req->absoluteURI << " |=============="<< std::endl;
	return req->absoluteURI;
}

bool response::methode_allowded(std::string methode)
{
	if(config->location[indexLocation].methods.empty())
		return false;
	for(size_t i = 0; i < config->location[indexLocation].methods.size(); i++)
			if(config->location[indexLocation].methods[i] == methode)
				return true;
	return false;
}


std::string response::redirection()
{
	return generateResponseHeader("text/html", std::to_string(status_code(301).size()), 301) + status_code(301);
}

void response::replacee(std::string &s, std::string amlo, std::string argan)
{
	int index = 0;
	int i;
	index = s.find(amlo);
	if (index == -1)
		return;
	s.erase(index, amlo.length());
	i = index;
	replacee(s, amlo, argan);
	s.insert(i, argan);
}

std::string response::checkURI(std::string &URI)
{
	if (URI.find("%20") != std::string::npos)
		replacee(URI, "%20", " ");
	else if (URI.find("%22") != std::string::npos)
		replacee(URI, "%22", "\"");
	else if (URI.find("%3c") != std::string::npos)
		replacee(URI, "%3c", "<");
	else if (URI.find("%3e") != std::string::npos)
		replacee(URI, "%3e", ">");
	else if (URI.find("%23") != std::string::npos)
		replacee(URI, "%23", "#");
	else if (URI.find("%25") != std::string::npos)
		replacee(URI, "%25", "%");
	else if (URI.find("%7b") != std::string::npos)
		replacee(URI, "%7b", "{");
	else if (URI.find("%7d") != std::string::npos)
		replacee(URI, "%7d", "}");
	else if (URI.find("%7c") != std::string::npos)
		replacee(URI, "%7c", "|");
	else if (URI.find("%5c") != std::string::npos)
		replacee(URI, "%5c", "\\");
	else if (URI.find("%5e") != std::string::npos)
		replacee(URI, "%5e", "^");
	else if (URI.find("%7e") != std::string::npos)
		replacee(URI, "%7e", "~");
	else if (URI.find("%5b") != std::string::npos)
		replacee(URI, "%5b", "[");
	else if (URI.find("%5d") != std::string::npos)
		replacee(URI, "%5d", "]");
	else if (URI.find("%60") != std::string::npos)
		replacee(URI, "%60", "`");
	return URI;
}

std::string   response::get_response()
{
	std::map<std::string, std::string>::iterator it = req->data.find("Transfer-Encoding:");
	if(it != req->data.end() && req->data["Transfer-Encoding:"] != "chunked")
		return generateResponse(501);
	else if(req->method == "POST" && it == req->data.end())
	{
		std::map<std::string, std::string>::iterator it = req->data.find("Content-Length:");
		if(it == req->data.end())
			return generateResponse(400);
	}
	if(req->absoluteURI.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%") != std::string::npos)
		return generateResponse(400);
	else if(req->absoluteURI.size() > 2048)
		return generateResponse(414);
	else if(req->body.size() > config->client_max_body_size)
		return generateResponse(413);
	req->absoluteURI = checkURI(req->absoluteURI);
	if(matchLocation().empty())
		return generateResponse(404);
	std::pair<std::string, std::string>p2("File_Name:", req->absoluteURI.substr(0, req->absoluteURI.size()));
	req->data.insert(p2);
	if(!config->location[indexLocation].redirection.empty())
		return redirection();
	if(req->method == "GET")
		return GET();
	else if(req->method == "POST")
		return POST();
	else if(req->method == "DELETE")
			return DELETE();
	return generateResponse(404);
}

std::string	response::status_code(int status_code)
{
	if(status_code == 200)
		return "HTTP/1.1 200 Ok\r\n";
	else if(status_code == 201)
		return "HTTP/1.1 201 Created\r\n";
	else if(status_code == 204)
		return "HTTP/1.1 204 No Content\r\n";
	else if(status_code == 301)
		return "HTTP/1.1 301 Moved Permanently\r\n";
	else if(status_code == 400)
		return "HTTP/1.1 400 Bad Request\r\n";
	else if(status_code == 403)
		return "HTTP/1.1 403 Forbidden\r\n";
	else if(status_code == 404)
		return "HTTP/1.1 404 Not Found\r\n";
	else if(status_code == 405)
		return "HTTP/1.1 405 Method Not Allowed\r\n";
	else if(status_code == 408)
		return "HTTP/1.1 408 Request Timeout\r\n";
	else if(status_code == 409)
		return "HTTP/1.1 409 Conflict\r\n";
	else if(status_code == 413)
		return "HTTP/1.1 413 Request Entity Too Large\r\n";
	else if(status_code == 414)
		return "HTTP/1.1 414 Request-URI Too Long\r\n";
	else if(status_code == 501)
		return "HTTP/1.1 501 Not Implemented\r\n";
	return NULL;
}

std::string response::generateStatusPages(int code)
{
	std::ifstream file;
	file.open("./status_pages/" + std::to_string(code) + ".html");
	std::ostringstream content;
	content << file.rdbuf();
	return content.str();
}

std::string	response::kk(std::string code)
{
	if(indexLocation == -1)
		return "";
	std::string path;
	std::cout << "=======================Error Page=====================\n";
	for(size_t i = 0; i < config->location[indexLocation].error_page.size() ; i++){
		for (size_t j = 0; j < config->location[indexLocation].error_page[i].first.size(); j++){
			std::cout << "========Error " << config->location[indexLocation].error_page[i].first[j] << std::endl;
			if (config->location[indexLocation].error_page[i].first[j] == code)
			{
				path = config->location[indexLocation].root + config->location[indexLocation].error_page[i].second + "/" + code + ".html";
				path = cleanupURI(path);
				std::cout << "========ERROR " << path << std::endl;
				return cleanupURI(path);
			}
		}
	}
	return "";
}
std::string	response::generateResponse(int code)
{
	std::string error_page = kk(std::to_string(code));
	std::ifstream file;
	file.open(error_page);
	if(!file)
		file.open("./status_pages/" + std::to_string(code) + ".html");
	std::ostringstream content;
	content << file.rdbuf();
	std::string body = content.str();
	return generateResponseHeader("text/html", std::to_string(body.size()), code) + body;
}

response::~response(){
	delete this->config;
}