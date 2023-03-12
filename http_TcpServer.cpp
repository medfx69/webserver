#include "http_TcpServer.hpp"
void log(const std::string & message){
    std::cout << message<< std::endl;
}
void exitWithError(const std::string& message){
    std::cerr << message << std::endl;
    exit(1);
}


http::TcpServer::TcpServer(std::string ip_address, int port):m_ip_address(ip_address),
                                                                m_port(port), m_socket(), m_new_socket(), m_incomingMessage(),
                                                                m_socketAress(), m_socketAddress_len(sizeof(m_socketAress)),
                                                                m_serverMessage(buildResponse())
{
    m_socketAress.sin_family = AF_INET;
    m_socketAress.sin_port = htons(m_port);
    m_socketAress.sin_addr.s_addr = inet_addr(m_ip_address.c_str());

}

http::TcpServer::~TcpServer()
{
    closeServer();
}
int http::TcpServer::startServer(){
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket < 0){
        exitWithError("Cannot create socket");
        return 1;
    }
    if (bind(m_socket, (sockaddr *) &m_socketAress, m_socketAddress_len) < 0){
        exitWithError("Cannot connect socket to address");
        return 1;
    }
    return 0;
}
void http::TcpServer::acceptConnection(SOCKET &new_socket){
    new_socker = accept(m_socket, (sockaddr *)&m_socketAress, &m_socketAddress_len);
    if (new_socket < 0){
        std::ostringstream ss;
        ss <<
        "Server failed to accept incoming connection from ADDRESS: "
        << inet_ntoa(m_socketAress.sin_addr) << "; PORT: "
        << ntohs (m_socketAress.sin_port);
        exitWithError(ss.str());
    }
}
void http::TcpServer::startListen(){
    if (listen(m_socket, 20) < 0){
        exitWithError("Socket listen failed");
    }
    std::ostringstream ss;
    ss << "\n***** Listening on ADDRESS: "
        << inet_ntoa(m_socketAress.sin_addr)
        << " PORT:" << ntohs(m_socketAress.sin_port)
        << "*****\n\n";
    log(ss.str());
}
int http::TcpServer::closeServer(){
    close(m_socket);
    close(m_new_socket);
    exit(0);
}