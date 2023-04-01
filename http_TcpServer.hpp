#ifndef HTTP_TCP_SERVER_HPP
#define HTTP_TCP_SERVER_HPP
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sys/select.h>
#include <sys/types.h>
#include <sstream>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#define BUFFER_SIZE 400000

class Parsed;
void pars_request(Parsed *data);

namespace http
{
    class TcpServer
    {
        private:
            std::string         m_ip_address;
            int                 m_socket;
            int                 m_new_socket;
            int                 m_port;
            long                m_incomingMessage;
            struct sockaddr_in  m_socketAress;
            unsigned int        m_socketAddress_len;
            std::string         m_serverMessage;
            int                 startServer();
            int                 closeServer();
        public:
            TcpServer(std::string ip_address, int port);
            ~TcpServer();
            void        startListen(Parsed *data);
            void        acceptConnection(int &new_socket);
            std::string buildResponse();
            void        sendResponse();
    };
} // namespace http`
void exitWithError(const std::string& message);


#endif