#ifndef REQUEST_HPP
#define REQUEST_HPP
#include "includes.hpp"

class request;

struct client
{
    std::string     client_resFile;
    std::string     client_res_message;
    request         *req;
    int             read_status;
    size_t          write_sened;
    size_t          write_len;
    size_t          readed;
    int             flag;
    size_t          read_len;
    int             fd_enabeld;
    int             serverIndex;
    int             client_fd;
    int             chunked;
    client(std::string file, int r_status, int w_status, int en, int index,int fd):client_resFile(""),
        read_status(r_status), write_sened(0), write_len(0), readed(0), flag(0), read_len(0),
        fd_enabeld(en), serverIndex(index), client_fd(fd), chunked(0){
            (void) file;
            (void) w_status;
        }
};

class request
{
public:
    std::string method;
    std::string absoluteURI;
    std::string http_version;
    std::string body;
    std::map<std::string, std::string> data;
    request(client *cl, std::string req);
    request();
    void handle_body(client *cl, std::string req);
};

#endif