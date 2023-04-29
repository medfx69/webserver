#ifndef HTTP_TCP_SERVER_HPP
#define HTTP_TCP_SERVER_HPP
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include "parsing/includes.hpp"
#include <sys/select.h>
#include <sys/types.h>
#include <sstream>
#include <string>
#include <iterator>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include "methods/http_response.hpp"
#define BUFFER_SIZE 400000

 
struct clinte
{
    int clinte_fd;
    Parsed _pr;
    std::string clinte_reqFile;
    std::string clinte_resFile;
    clinte(Parsed pr, std::string file, int fd):_pr(pr), clinte_reqFile(file), clinte_fd(fd){}
};

struct server_fd
{
    int socket_fd;
    int max_fd;
    fd_set writeset, readset;
    server_fd(int fd){
        socket_fd = fd;
        max_fd = fd;
        FD_SET(fd, &readset);
        FD_ZERO(&writeset);
    }
}; 

class response;

namespace http
{
    class TcpServer
    {
        private:
            Parsed *_data;
            response *resp;
            std::vector<std::string>            m_ip_address;
            std::vector<int>                    m_port;
            std::vector<int>                    m_socket;
            std::vector<server_fd>              serverFd;
            std::vector<int>                    m_new_socket;
            std::vector<clinte>                 clintes;
            std::string                         m_serverMessage;
            fd_set                              readst, writest;
            long                                m_incomingMessage;
            struct sockaddr_in                  m_socketAress;
            unsigned int                        m_socketAddress_len;
            int                                 startServer();
            int                                 closeServer();
        public:
            TcpServer(Parsed *data);
            ~TcpServer();
            void                                startListen(Parsed *data);
            int                                 acceptConnection(int fd);
            void                                buildResponse(Parsed *data);
            void                                sendResponse(int fd);
    };
} // namespace http`
void exitWithError(const std::string& message);


#endif