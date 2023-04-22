#include "http_TcpServer.hpp"

void log(const std::string &message)
{
    std::cout << message << std::endl;
}
void exitWithError(const std::string &message)
{
    std::cerr << message << std::endl;
    exit(1);
}

http::TcpServer::TcpServer(Parsed *data) : _data(data), m_socket(), m_new_socket(), m_incomingMessage(),
                                                               m_socketAress(),
                                                               m_serverMessage(buildResponse())
{
    std::vector<server>::iterator it;
    for(it = _data->getDate().begin(); it < _data->getDate().end(); it++){
        if ((*it).listen.first.find('.') != std::string::npos){
            m_ip_address.push_back((*it).listen.first);
            m_port.push_back(std::stoi((*it).listen.second));
        }
        else{
            m_port.push_back(std::stoi((*it).listen.first));
            m_ip_address.push_back((*it).server_name);
        }
        std::cout << *(m_port.end() -1) << std::endl;
        std::cout << *(m_ip_address.end() - 1) << std::endl;
        m_socketAress.sin_family = AF_INET;
        m_socketAress.sin_port = htons(*(m_port.end() - 1));
        m_socketAress.sin_addr.s_addr = inet_addr((*(m_ip_address.end() - 1)).c_str());
        m_socketAddress_len = sizeof(m_socketAress);
        if (startServer() != 0)
        {
            std::ostringstream ss;
            ss << "Failed to start with PORT:" << ntohs(m_socketAress.sin_port);
            log(ss.str());
        }
    }
}

http::TcpServer::~TcpServer()
{
    closeServer();
}

int http::TcpServer::startServer()
{
    int i = 1;

    m_socket.push_back(socket(AF_INET, SOCK_STREAM, 0));
    if (*(m_socket.end() - 1) < 0)
        exitWithError("Cannot create socket");
    if (setsockopt(*(m_socket.end() - 1), SOL_SOCKET, SO_REUSEADDR, (char *)&i, sizeof(i)) < 0)
        exitWithError("----------setsockopt-----------");
    if (bind(*(m_socket.end() - 1), (sockaddr *)&m_socketAress, m_socketAddress_len) < 0)
        exitWithError("Cannot connect socket to address");
    return 0;
}

int http::TcpServer::acceptConnection(int fd)
{
    int new_socket;
    new_socket = accept(fd, (sockaddr *)&m_socketAress, &m_socketAddress_len);
    if (new_socket < 0)
    {
        std::ostringstream ss;
        ss << "Server failed to accept incoming connection from ADDRESS: "
           << inet_ntoa(m_socketAress.sin_addr) << "; PORT: "
           << ntohs(m_socketAress.sin_port);
        exitWithError(ss.str());
    }
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

    for (std::vector<int>::iterator it = m_socket.begin(); it < m_socket.end(); it++)
        if (listen(*it, SOMAXCONN) < 0)
            exitWithError("Socket listen failed");
    max_fd = *(m_socket.end() - 1);
    max_fd_tmp = max_fd;
    timer.tv_sec = 40;
    for (std::vector<int>::iterator it = m_socket.begin(); it < m_socket.end(); it++)
        FD_SET(*it, &readst);
    while (true)
    {
        for (std::vector<int>::iterator it = m_socket.begin(); it < m_socket.end(); it++){
            read_tmp = readst;
            write_tmp = writest;
            // log("====== Waiting for a new connection ======\n\n\n");
            act = select(max_fd +1, &readst, &writest, NULL, &timer);
            std::cout << act << std::endl;
            if (act < 0)
                exitWithError("--------select error-------");
            for (int i = 0; i < max_fd +1 ; i++){
                if (FD_ISSET(i, &readst) || FD_ISSET(i, &writest)){
                    if (i == *it){
                        max_fd_check = acceptConnection(*it);
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
                            FD_CLR(i, &write_tmp);
                        }
                        else if (bytesReceived == 0)
                            close(i);
                    }
                }
            }
            writest = write_tmp;
            readst = read_tmp;
            max_fd = max_fd_tmp;
        }
    }
}

int http::TcpServer::closeServer()
{
    for (std::vector<int>::iterator ite = m_socket.begin(); ite < m_socket.end(); ite++)
        close(*ite);
    // close(*(m_new_socket.end() - 1));
    exit(0);
}

std::string http::TcpServer::buildResponse()
{
    std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :)</body></html>";
    std::ostringstream ss;

    ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
       << htmlFile;
    return ss.str();
}

void http::TcpServer::sendResponse(int fd)
{
    long bytesSent;

        
    write(fd, m_serverMessage.c_str(), m_serverMessage.size());
    // if (bytesSent == m_serverMessage.size())
    //     log("------ Server Response sent to client ------\n\n");
    // else
    //     log("Error sending response to client");
}
