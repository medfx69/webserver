#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <dirent.h>
#include <sys/stat.h>
#include "../parsing/includes.hpp"
#include "../http_TcpServer.hpp"

struct del
{
  public:
    std::string filename;
    std::string foldername;
};

class response
{
	private:
		std::map<std::string, std::string> mimeTypeMap;
		std::map<std::string, std::string> mimeTypeMap2;
		int indexLocation;
		request *req;
		server *config;
		std::string content_type;
		int content_lenght;
		std::string	status;
		std::ifstream bodyfile;
		std::map<std::string ,std::string> boundray;
		std::string Bbody;
	public:
		//get
		std::string GET();
		response (request* _req,server _config);
		~response();
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
		std::string generateStatusPages(int status_code);
		std::string	generateResponse(int code);
		bool 		methode_allowded(std::string methode);
		std::string cleanupURI(std::string& uri);
		std::string redirection();
		//post
		std::string	POST();
		int	addboundaryheader(std::string &);
		void	uploadbody();
		//delete
		std::string	DELETE();
		bool 		fd_remove(std::string path);
		bool 		checkPermission(std::string path, std::string i);
		//cgi
		std::string exec_outfile(std::string inFile, std::map<std::string, std::string> reqHeader);
		std::string checkURI(std::string &URI);
		void 		replacee(std::string &s, std::string amlo, std::string argan);
		std::string		cgi__();
};

#endif