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

http::TcpServer::TcpServer(Parsed *data) : _data(data), m_socket(), m_new_socket()
{
    struct sockaddr_in                  m_socketAress;

    for(size_t i = 0; i < _data->getDate().size(); i++){
        if ((_data->getDate()[i]).listen.first.find('.') != std::string::npos){
            m_ip_address.push_back((_data->getDate()[i]).listen.first);
            m_port.push_back(std::stoi((_data->getDate()[i]).listen.second));
        }
        else{
            m_port.push_back(std::stoi((_data->getDate()[i]).listen.first));
            m_ip_address.push_back((_data->getDate()[i]).server_name);
        }
        m_socketAress.sin_family = AF_INET;
        m_socketAress.sin_port = htons(m_port[i]);
        m_socketAress.sin_addr.s_addr = inet_addr(m_ip_address[i].c_str());
        // m_socketAress.sin_addr.s_addr = INADDR_ANY;
        m_socketAddress_len.push_back(sizeof(m_socketAress));
        class_m_socketAress.push_back(m_socketAress);
    }
        if (startServer() != 0)
        {
            std::ostringstream ss;
            ss << "Failed to start with PORT:" << ntohs(m_socketAress.sin_port);
            log(ss.str());
        }
}

http::TcpServer::~TcpServer()
{
    closeServer();
}

int http::TcpServer::startServer()
{
    int i = 1;
    int option = 1;
    for (size_t j = 0; j < class_m_socketAress.size(); j++){
        i = socket(AF_INET, SOCK_STREAM, 0);
        std::cout << "fd1 = " << i << std::endl;
        if (i < 0)
            exitWithError("Cannot create socket");
        m_socket.push_back(i);
        if (setsockopt(i, SOL_SOCKET, SO_REUSEADDR , &option, sizeof(option)) < 0)
            exitWithError("----------setsockopt-----------");
        // if (setsockopt(i, SOL_SOCKET, SO_NOSIGPIPE , &option, sizeof(option)) < 0)
        //     exitWithError("----------setsockopt-----------");
        if (bind(i, (struct sockaddr*)&class_m_socketAress[j], m_socketAddress_len[j]) < 0){
            exitWithError("Cannot connect socket to address");
        }
    }
    return 0;
}

int setNonblocking(int fd)
{
    return fcntl(fd, F_SETFL, O_NONBLOCK);
}

int http::TcpServer::acceptConnection(int fd, int c)
{
    int new_socket;
    new_socket = accept(fd, (sockaddr *)&class_m_socketAress[c], &m_socketAddress_len[c]);
    if (new_socket < 0)
    {
        std::ostringstream ss;
        ss << "Server failed to accept incoming connection from ADDRESS: "
           << inet_ntoa(class_m_socketAress[c].sin_addr) << "; PORT: "
           << ntohs(class_m_socketAress[c].sin_port);
        exitWithError(ss.str());
    }
    setNonblocking(new_socket);
    m_new_socket.push_back(new_socket);
    return new_socket;
}


int http::TcpServer::listening(){
    int max_fd;

    for (std::vector<int>::iterator it = m_socket.begin(); it < m_socket.end(); it++)
        if (listen(*it, SOMAXCONN) < 0)
            exitWithError("Socket listen failed");
    timer.tv_sec = 4;
    timer.tv_usec = 0;
    FD_ZERO(&readst);
    FD_ZERO(&writest);
    max_fd = 0;
    for (std::vector<int>::iterator ite = m_socket.begin(); ite < m_socket.end(); ite++)
    {
        FD_SET(*ite, &readst);
        max_fd = *ite;
        std::cout << max_fd << std::endl;
    }
    return max_fd;
}

void http::TcpServer::save(int fd){
    std::ostringstream  ss1;

    ss1 << "./usefull_files/request_" << fd;
    std::ofstream reFile(ss1.str());
    reFile << buffer;
    reFile.close();
}

void http::TcpServer::startListen(Parsed *data){
    int                 bytesReceived, max_fd, max_fd_tmp, act, max_fd_check;
    fd_set              read_tmp, write_tmp;
    std::ostringstream  ss;

    max_fd = listening();
    FD_ZERO(&read_tmp);
    FD_ZERO(&write_tmp);
    max_fd_tmp = max_fd;
    while (true)
    {
        int c = 0;
        // for (std::vector<int>::iterator it = m_socket.begin(); it < m_socket.end(); it++){
            // std::cout << ">>>>" << *it << std::endl;
            read_tmp = readst;
            write_tmp = writest;
            log("====== Waiting for a new connection ======\n\n\n");
            act = select(max_fd +1, &readst, &writest, NULL, NULL);
            if (act < 0)
                exitWithError("--------select error-------");
            std::vector<std::pair<int, bool> > fds;
            for (int i = 0; i < max_fd + 1 ; i++){
                if (FD_ISSET(i, &read_tmp) || FD_ISSET(i, &write_tmp)){
                    if (i == m_socket[0]){
                        fds.push_back(std::make_pair(i, true));
                        max_fd_check = acceptConnection(m_socket[0], c);
                        FD_SET(max_fd_check, &read_tmp);
                        if (max_fd_check > max_fd_tmp)
                            max_fd_tmp = max_fd_check;
                    }
                    else{
                        fds.push_back(std::make_pair(i, false));
                        bytesReceived = recv(i, buffer, BUFFER_SIZE, 0);
                        save(i);
                        FD_SET(i, &write_tmp);
                        FD_CLR(i, &read_tmp);
                        (void)data;
                        buildResponse(data);
                        std::ostringstream ss;
                        log(ss.str());
                        sendResponse(i);
                        FD_CLR(i, &write_tmp);
                        // close(i);

                    }                  
                }
            // }
            for (size_t x = 0; x < fds.size(); x++){
                std::cout << fds[x].first << ":" << fds[x].second << ";";
            }
            std::cout << std::endl;
            c++;
            max_fd = max_fd_tmp;
            readst = read_tmp;
            writest = write_tmp;
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

void http::TcpServer::buildResponse(Parsed *data)
{
    // std::cout << ">>>>>" <<  data->req->method<< std::endl;
    // if(data->req->method == "GET") {
    //     m_serverMessage = resp->get_response(data);
    //     return ;
    // }
    // else if(data->req->method == "DELETE")
    //     ;
    // else if(data->req->method == "POST")
    //     ;
    // else {
    //     std::cerr << "Unsupported HTTP method: " << data->req->method << '\n';
    //     this->m_serverMessage = "HTTP/1.1 405 Method Not Allowed\r\n\r\n";
    //     return ;
    // }
    (void) data;
    (void) resp;
    std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :)</body></html>";
    std::ostringstream ss;
    ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n" << htmlFile;
    this->m_serverMessage = ss.str();
}

void http::TcpServer::sendResponse(int fd)
{
    write(fd, m_serverMessage.c_str(), m_serverMessage.size());
    // if (bytesSent == m_serverMessage.size())
    //     log("------ Server Response sent to client ------\n\n");
    // else
    //     log("Error sending response to client");
}
