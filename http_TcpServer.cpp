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
                                                                m_serverMessage(buildResponse()){
    m_socketAress.sin_family = AF_INET;
    m_socketAress.sin_port = htons(m_port);
    m_socketAress.sin_addr.s_addr = inet_addr(m_ip_address.c_str());
    if (startServer() != 0){
        std::ostringstream ss;
        ss << "Failed to start with PORT:" << ntohs(m_socketAress.sin_port);
        log(ss.str());
    }

}

http::TcpServer::~TcpServer(){
    closeServer();
}


int http::TcpServer::startServer(){
    int i = 1;

    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket < 0)
        exitWithError("Cannot create socket");
    if(setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&i, sizeof(i)) < 0 )
        exitWithError("----------setsockopt-----------");
    if (bind(m_socket, (sockaddr *) &m_socketAress, m_socketAddress_len) < 0)
        exitWithError("Cannot connect socket to address");
    return 0;
}


int http::TcpServer::acceptConnection(){
    int new_socket;
    new_socket = accept(m_socket, (sockaddr *)&m_socketAress, &m_socketAddress_len);
    // if (new_socket < 0){
    //     std::ostringstream ss;
    //     ss <<
    //     "Server failed to accept incoming connection from ADDRESS: "
    //     << inet_ntoa(m_socketAress.sin_addr) << "; PORT: "
    //     << ntohs (m_socketAress.sin_port);
    //     exitWithError(ss.str());
    // }
    return new_socket;
}

int setNonblocking(int fd)
{
    int flags;

    if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
        flags = 0;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}


void http::TcpServer::startListen(Parsed *data){
    int                 bytesReceived, max_fd, max_fd_tmp, act, max_fd_check;
    fd_set              read_tmp, write_tmp;
    std::ostringstream  ss;
    timeval             timer;

    if (listen(m_socket, SOMAXCONN) < 0)
        exitWithError("Socket listen failed");
    ss << "\n***** Listening on ADDRESS: "
        << inet_ntoa(m_socketAress.sin_addr)
        << " PORT:" << ntohs(m_socketAress.sin_port)
        << "*****\n\n";
    log(ss.str());

    FD_ZERO(&readst);
    FD_ZERO(&writest);
    max_fd = m_socket;
    max_fd_tmp = m_socket;
    timer.tv_sec = 40;
    FD_SET(m_socket, &readst);
    while (true)
    {
        read_tmp = readst;
        write_tmp = writest;
        log("====== Waiting for a new connection ======\n\n\n");
        act = select(max_fd +1, &readst, &writest, NULL, NULL);
        if (act < 0)
            exitWithError("--------select error-------");
        for (int i = 0; i < max_fd +1 ; i++){
            if (FD_ISSET(i, &readst) || FD_ISSET(i, &writest)){
                if (i == m_socket){
                    max_fd_check = acceptConnection();
                    FD_SET(max_fd_check, &read_tmp);
                    m_new_socket.push_back(max_fd_check);
                    if (max_fd_check > max_fd_tmp)
                        max_fd_tmp = max_fd_check;
                    setNonblocking(max_fd_check);
                }
                else{
                    char buffer[BUFFER_SIZE] = {0};
                    bytesReceived = read(i, buffer, BUFFER_SIZE);
                    if (bytesReceived >= 0){
                        std::ostringstream  ss1;
                        ss1 << "./usefull_files/request_" << i;
                        std::ofstream reFile(ss1.str());
                        reFile << buffer;
                        reFile.close();
                        FD_SET(i, &write_tmp);
                        FD_CLR(i, &read_tmp);
                        pars_request(data);
                        std::ostringstream ss;
                        ss << "------ Received Request from client ------\n\n";
                        log(ss.str());
                        if (FD_ISSET(i, &write_tmp))
                            sendResponse(i);
                        FD_CLR(i, &writest);
                    }
                }
            }
        }
        writest = write_tmp;
        readst = read_tmp;
        max_fd = max_fd_tmp;
    }
}


int http::TcpServer::closeServer(){
    close(m_socket);
    close(*(m_new_socket.end() -1));
    exit(0);
}

std::string http::TcpServer::buildResponse(){
    std::string         htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :)</body></html>";
    std::ostringstream  ss;
    
    ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
       << htmlFile;
    return ss.str();

}

 
void http::TcpServer::sendResponse(int fd){
    long    bytesSent;

        bytesSent = write(fd, m_serverMessage.c_str(), m_serverMessage.size());
        if (bytesSent > 0){
            FD_CLR(fd, &writest);
            // close(fd);
        }
    // if (bytesSent == m_serverMessage.size())
    //     log("------ Server Response sent to client ------\n\n");
    // else
    //     log("Error sending response to client");
}

