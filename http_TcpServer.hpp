#ifndef HTTP_TCP_SERVER_HPP
#define HTTP_TCP_SERVER_HPP
#include <sys/socket.h>
#include <unistd.h>
#include "parsing/includes.hpp"
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sstream>
#include <string>
#include <stdio.h>
#include <iterator>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include "methods/http_response.hpp"
#define BUFFER_SIZE 100000

class response;

namespace http
{
    class TcpServer
    {
    private:
        Parsed *_data;
        std::vector<std::string> m_ip_address;
        std::vector<std::pair<std::string, int> > server_names;
        std::vector<int> m_port;
        std::vector<int> m_a_port;
        std::vector<int> m_socket;
        std::vector<int> m_socket_c;
        std::vector<std::vector<int> > m_socket_c_a;
        std::vector<int> m_new_socket;
        std::vector<client> clients;
        struct sockaddr_in m_socketAress;
        int m_socketAddress_len;
        std::string m_serverMessage;
        char buffer[BUFFER_SIZE];
        fd_set readst, writest;
        std::vector<struct sockaddr_in> class_m_socketAress;
        timeval timer;
        std::vector<std::vector<int> > indexing;
        std::vector<int> findexing;
        std::vector<unsigned int> class_m_socketAddress_len;
        int startServer();
        int closeServer();

    public:
        TcpServer(Parsed *data);
        ~TcpServer();
        int listening();
        int chekPort(int p, size_t i);
        int findIndex(int fd);
        bool isMaster(int fd);
        void save(int fd, int client, int size);
        void startListen(Parsed *data);
        int acceptConnection(int fd);
        void buildResponse(Parsed *data, int cl);
        int sendResponse(int fd);
        void reindexing(client &);
    };
}
void exitWithError(const std::string &message);

#endif