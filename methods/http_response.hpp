#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <dirent.h>
#include <sys/stat.h>
#include "../parsing/includes.hpp"
#include "../http_TcpServer.hpp"

class response
{
	private:
		std::map<std::string, std::string> mimeTypeMap;
		int indexLocation;
		request *req;
		server *config;
		std::string content_type;
		int content_lenght;
		std::string	status;
	public:
		//get
		response (request* _req,server _config);
		std::string	get_response();
		std::string	getfolder();
		std::string	getfile();
		std::string	checkPathType();
		std::string createIndexHtml();
		std::string getFileExtension();
		std::string generateResponseHeader(std::string content_type, std::string content_lenght, int code);
		std::string contentType();
		std::string	matchLocation();
		std::string	status_code(int );
		std::string	get_date();
		std::string generateErrorPages(int status_code);
		std::string	errorPage(int code);
		bool 		methode_allowded(std::string methode);
		//delete
		bool folder_exists(std::string folder_path);
		std::string path_creat(std::string folder_path, std::string join_path);
		void  f_remove(std::string path);
		//post
		std::string	post();
};

#endif