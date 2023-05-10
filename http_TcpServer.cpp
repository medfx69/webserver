#include "http_TcpServer.hpp"
#define IMHERE std::cout  <<__FILE__ <<":"<<__LINE__ << " executed\n";
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
    readStatus = 0;
    writeStatus = 0;
    for (size_t i = 0; i < _data->getDate().size(); i++){
        if ((_data->getDate()[i]).listen.first.find('.') != std::string::npos){
            m_ip_address.push_back((_data->getDate()[i]).listen.first);
            m_port.push_back(std::stoi((_data->getDate()[i]).listen.second));
        }
        else{
            m_port.push_back(std::stoi((_data->getDate()[i]).listen.first));
            m_ip_address.push_back((_data->getDate()[i]).server_name);
        }
        m_socketAress.sin_family = AF_INET;
        std::cout << "port :" << m_port[i] << std::endl;
        m_socketAress.sin_port = htons(m_port[i]);
        std::cout << "addres :" << m_ip_address[i] << std::endl;
        m_socketAress.sin_addr.s_addr = inet_addr(m_ip_address[i].c_str());
        m_socketAddress_len = sizeof(m_socketAress);
        class_m_socketAddress_len.push_back(m_socketAddress_len);
        class_m_socketAress.push_back(m_socketAress);
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
    int fd;
    int i = 1;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
        exitWithError("Cannot create socket");
    if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&i, sizeof(i)) < 0 )
        exitWithError("----------setsockopt-----------");
    if (bind(fd, (sockaddr *) &m_socketAress, m_socketAddress_len) < 0){
        std::cout << errno << std::endl;
        exitWithError("Cannot connect socket to address");
    }
    m_socket.push_back(fd);
    return 0;
}

int setNonblocking(int fd)
{
    return fcntl(fd, F_SETFL, O_NONBLOCK);
}

int http::TcpServer::acceptConnection(int fd, int c)
{
    int new_socket;

    m_socketAress = class_m_socketAress[c];
    m_socketAddress_len = class_m_socketAddress_len[c];
    std::cout << inet_ntoa(m_socketAress.sin_addr) << std::endl;
    std::cout << ntohs(m_socketAress.sin_port) << std::endl;
    IMHERE
    std::cout << "ACCEPTED FROM: " << fd << std::endl;
    new_socket = accept(fd, (sockaddr *)&m_socketAress, (socklen_t *) &m_socketAddress_len);
    IMHERE
    if (new_socket < 0)
    {
        std::cout << errno << std::endl;
        std::ostringstream ss;
        ss << "Server failed to accept incoming connection from ADDRESS: "
           << inet_ntoa(class_m_socketAress[c].sin_addr) << "; PORT: "
           << ntohs(class_m_socketAress[c].sin_port);
        exitWithError(ss.str());
    }
    else{
        setNonblocking(new_socket);
        m_new_socket.push_back(new_socket);
    }
    return new_socket;
}


int http::TcpServer::listening(){
    int max_fd;

    max_fd = 0;
    FD_ZERO(&readst);
    for (size_t i = 0; i < m_socket.size(); i++){
        std::cout << m_socket[i] << std::endl;
        if (listen(m_socket[i], SOMAXCONN) < 0)
            exitWithError("Socket listen failed");
        FD_SET(m_socket[i], &readst);
        max_fd = m_socket[i];
    }
    FD_ZERO(&writest);
    timer.tv_sec = 4;
    timer.tv_usec = 0;
    return max_fd;
}

void http::TcpServer::save(int fd, int client){
    std::ostringstream  ss1;

    ss1 << "/tmp/request_" << fd;
    std::ofstream reFile(ss1.str());

    reFile << buffer;
    // if (status == 1)
        reFile.close();
    clients[client]._pr.req = pars_request(clients[client]._pr, clients[client].client_fd, &this->clients[client].read_status);
}

bool http::TcpServer::isMaster(int fd){
    for (size_t i = 0; i < m_socket.size(); i++)
        if(fd == m_socket[i])
            return true;
    return (false);
}

void http::TcpServer::startListen(Parsed *data){
    int                 bytesReceived, max_fd, max_fd_tmp, act, max_fd_check;
    fd_set              read_tmp, write_tmp;
    std::ostringstream  ss;

    FD_ZERO(&read_tmp);
    FD_ZERO(&write_tmp);
    max_fd = listening();
    max_fd_tmp = max_fd;
    while (true)
    {
        // system("lsof -i tcp | grep webserv");
        // IMHERE
        read_tmp = readst;
        write_tmp = writest;
        log("====== Waiting for a new connection ======\n");
        act = select(max_fd +1, &readst, &writest, NULL, NULL);
        for (size_t c = 0; c < m_socket.size(); c++){
           
            // IMHERE
            if (act < 0)
                exitWithError("--------select error-------");
            for (int i = 0; i < max_fd + 1 ; i++){
                if (FD_ISSET(i, &read_tmp))
                {
                    // IMHERE
                    if (isMaster(i)){
                        // IMHERE
                        max_fd_check = acceptConnection(i, c);
                        FD_SET(max_fd_check, &read_tmp);
                        if (max_fd_check > max_fd_tmp)
                            max_fd_tmp = max_fd_check;
                        size_t cl = 0;
                        for (; cl < clients.size(); cl++){
                            if (clients[cl].client_fd == max_fd_check){
                                clients[cl].read_status = 0;
                                clients[cl].write_status = 0;
                                clients[cl].fd_enabeld = 1;
                            }
                        }
                        if (cl == clients.size()){
                            std::ostringstream  ss1;
                            ss1 << "/tmp/request_" << max_fd_check;
                            clients.push_back(client(data[c], ss1.str(), 0, 0, 1, max_fd_check));
                        }
                    }
                    else{
                        // IMHERE
                        bytesReceived = read(i, buffer, BUFFER_SIZE);
                        if (bytesReceived >= 0)
                        {
                            size_t cl1 = 0;
                            // std::cout << "Read Return: " << bytesReceived << " {}" << buffer << std::endl;
                            // here i should pars requust and return status code
                            for (; cl1 < clients.size(); cl1++){
                                if (clients[cl1].client_fd == i){
                                    save(i, cl1);
                                    clients[cl1].read_status = status;
                                    clients[cl1].write_status = 0;
                                    clients[cl1].fd_enabeld = 1;
                                }
                            }
                            FD_SET(i, &write_tmp);
                            FD_CLR(i, &read_tmp);
                        }
                    }
                }
                if (FD_ISSET(i, &write_tmp))
                {
                    // IMHERE
                    buildResponse(&clients[c]._pr, c);
                    if (FD_ISSET(i, &write_tmp)){

                        if(sendResponse(i) > 0){
                            FD_CLR(i, &write_tmp);
                            close(i);
                        }
                    }
                    // IMHERE
                    // IMHERE
                    
                }
            }
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

void http::TcpServer::buildResponse(Parsed *data, int cl)
{
    static int i = 0;
    // if (data->req->method.empty())
    if(!i)
    {
        std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :)</body></html>";
        std::ostringstream ss;
        ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n" << htmlFile;
        this->m_serverMessage = ss.str();
        i++;
        return ;
    }
    std::cout << ">>>>>" << data->req->method<< std::endl;
    if(data->req->method == "GET") {
        m_serverMessage = resp->get_response(data, _data->getDate()[cl]);
        return ;
    }
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
}

int http::TcpServer::sendResponse(int fd)
{
    return (write(fd, m_serverMessage.c_str(), m_serverMessage.size()));
    // if (bytesSent == m_serverMessage.size())
    //     log("------ Server Response sent to client ------\n\n");
    // else
    //     log("Error sending response to client");
}

// lsof -n -i4TCP:8081