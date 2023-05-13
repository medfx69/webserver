
#ifndef REQUEST_HPP
#define REQUEST_HPP
#include "includes.hpp"

class request;

struct client
{
    std::string client_reqFile;
    std::string client_res_message;
    request     *req;
    int         read_status;
    size_t      write_sened;
    int         readed;
    int         flag;
    size_t      read_len;
    int         fd_enabeld;
    int         serverIndex;
    int         client_fd;
    int         chunked;
    client(std::string file, int r_status, int w_status, int en, int index,int fd):client_reqFile(file),
        read_status(r_status), write_sened(w_status), readed(0), flag(0), read_len(0),
        fd_enabeld(en), serverIndex(index), client_fd(fd), chunked(0){}
};

class request
{
public:
	std::string method;
	std::string absoluteURI;
	std::string http_version;
	std::string body;
	std::map<std::string, std::string> data;	
	request(client *cl);
	request();
    void    handle_body(client *cl);
};


#endif