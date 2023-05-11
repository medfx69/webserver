#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <dirent.h>
#include <sys/stat.h>
#include "../parsing/includes.hpp"
#include "../http_TcpServer.hpp"

class response
{
	private:
		int content_lenght;
		std::map<std::string, std::string> mimeTypeMap;
		// request req;
		// server config;
	public:
		// response (request , config);
		response ();
		std::string	get_response(request* data, server config);
		std::string	getfolder(request* data, server config);
		std::string	getfile(std::string pathfile);
		std::string	checkPathType(request* data);
		std::string createIndexHtml(std::string pathdir);
		std::string getFileExtension(const std::string& filePath);
		std::string generateResponseHeader(const std::string& filePath);
		std::string contentType(const std::string& filePath);
		std::string	matchLocation(request* req, server config);
		std::string	status_code(int );
		std::string	get_date();
};

#endif