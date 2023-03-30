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
    if (startServer() != 0){
        std::ostringstream ss;
        ss << "Failed to start with PORT:" << ntohs(m_socketAress.sin_port);
        log(ss.str());
    }

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


void http::TcpServer::acceptConnection(int &new_socket){
    new_socket = accept(m_socket, (sockaddr *)&m_socketAress, &m_socketAddress_len);
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
    int bytesReceived;
    int i = 1;
    if (listen(m_socket, SOMAXCONN) < 0){
        exitWithError("Socket listen failed");
    }
    std::ostringstream ss;
    ss << "\n***** Listening on ADDRESS: "
        << inet_ntoa(m_socketAress.sin_addr)
        << " PORT:" << ntohs(m_socketAress.sin_port)
        << "*****\n\n";
    log(ss.str());
    while (true)
    {
        log("====== Waiting for a new connection ======\n\n\n");
        acceptConnection(m_new_socket);
        if(setsockopt(m_new_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&i, sizeof(i)) < 0 )
            exitWithError("setsockopt");
        char buffer[BUFFER_SIZE] = {0};
        bytesReceived = read(m_new_socket, buffer, BUFFER_SIZE);
        if (bytesReceived < 0)
            exitWithError("Failed to read bytes from client socket connection");
        std::ofstream reFile("./usefull_files/request");
        std::cout << buffer;
        reFile << buffer;
        reFile.close();
        std::ostringstream ss;
        ss << "------ Received Request from client ------\n\n";
        log(ss.str());
        sendResponse();
        close(m_new_socket);
    }
}


int http::TcpServer::closeServer(){
    close(m_socket);
    close(m_new_socket);
    exit(0);
}

std::string http::TcpServer::buildResponse(){
    std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>";
    std::ostringstream ss;
    
    ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
       << htmlFile;
    return ss.str();

}

 
void http::TcpServer::sendResponse(){
    long bytesSent;

    bytesSent = write(m_new_socket, m_serverMessage.c_str(), m_serverMessage.size());
    if (bytesSent == m_serverMessage.size())
        log("------ Server Response sent to client ------\n\n");
    else
        log("Error sending response to client");
}

