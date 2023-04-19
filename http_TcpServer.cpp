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

void http::TcpServer::startListen(Parsed *data)
{
    int bytesReceived;
    int act;

    timeval timer;
    int max_fd;
    int max_fd_check;
    std::ostringstream ss;
    for (std::vector<int>::iterator it = m_socket.begin(); it < m_socket.end(); it++)
        if (listen(*it, SOMAXCONN) < 0)
            exitWithError("Socket listen failed");
    ss << "\n***** Listening on ADDRESS: "
       << inet_ntoa(m_socketAress.sin_addr)
       << " PORT:" << ntohs(m_socketAress.sin_port)
       << "*****\n\n";
    log(ss.str());

    FD_ZERO(&readst);
    FD_ZERO(&writest);
    max_fd = *(m_socket.end() -1);
    timer.tv_sec = 40;
    while (true)
    {
        for (std::vector<int>::iterator ite = m_socket.begin(); ite < m_socket.end(); ite++){
            FD_SET(*ite, &readst);
            log("====== Waiting for a new connection ======\n\n\n");
            act = select(max_fd + 1, &readst, &writest, NULL, NULL);
            if (act < 0)
                exitWithError("--------select error-------");
            else if (act == 0)
                log("--------time out-------");
            for (int i = 0; i < max_fd + 1; i++)
            {
                if (FD_ISSET(i, &readst) || FD_ISSET(i, &writest))
                {
                    if (i == *ite)
                    {
                        max_fd_check = acceptConnection(*ite);
                        int flags;
                        // setNonblocking(max_fd_check);
                        FD_SET(max_fd_check, &readst);
                        m_new_socket.push_back(max_fd_check);
                        if (max_fd_check > max_fd)
                            max_fd = max_fd_check;
                    }
                    else
                    {
                        for (std::vector<int>::iterator it = m_new_socket.begin(); it < m_new_socket.end(); it++)
                        {
                            if (FD_ISSET(*it, &readst))
                            {
                                char buffer[BUFFER_SIZE] = {0};
                                std::ostringstream ss1;
                                std::cout << "catshing " << *it << std::endl;
                                ss1 << "./usefull_files/request_" << *it;
                                std::ofstream reFile(ss1.str());
                                bytesReceived = read(i, buffer, BUFFER_SIZE);
                                if (bytesReceived < 0)
                                    exitWithError("Failed to read bytes from client socket connection");
                                reFile << buffer;
                                reFile.close();
                                if (*it != *ite)
                                {
                                    FD_SET(*it, &writest);
                                    FD_CLR(*it, &readst);
                                }
                                pars_request(data);
                                std::ostringstream ss;
                                ss << "------ Received Request from client ------\n\n";
                                log(ss.str());
                                sendResponse(*it);
                                FD_CLR(*it, &writest);
                                close(*it);
                            }
                        }
                    }
                }
            }
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

    if (FD_ISSET(fd, &writest))
        write(fd, m_serverMessage.c_str(), m_serverMessage.size());
    // if (bytesSent == m_serverMessage.size())
    //     log("------ Server Response sent to client ------\n\n");
    // else
    //     log("Error sending response to client");
}
