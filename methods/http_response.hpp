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
		request *req;
		server config;
	public:
		//get
		response (request* _req,server _config);
		std::string	get_response();
		std::string	getfolder();
		std::string	getfile();
		std::string	checkPathType();
		std::string createIndexHtml();
		std::string getFileExtension();
		std::string generateResponseHeader();
		std::string contentType();
		std::string	matchLocation();
		std::string	status_code(int );
		std::string	get_date();
		//delete
		bool folder_exists(std::string folder_path);
		std::string path_creat(std::string folder_path, std::string join_path);
		void  f_remove(std::string path);
};

#endif