#include "http_TcpServer.hpp"

#define IMHERE std::cout << __FILE__ << ":" << __LINE__ << " executed\n";
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
    for (size_t i = 0; i < _data->getDate().size(); i++)
    {
        for (size_t j = 0; j < _data->getDate()[i].listen.size(); j++)
        {
            m_ip_address.push_back((_data->getDate()[i]).listen[j].first);
            m_port.push_back(std::stoi((_data->getDate()[i]).listen[j].second));
            m_socketAress.sin_family = AF_INET;
            std::cout << "port :" << m_port[j] << std::endl;
            m_socketAress.sin_port = htons(m_port[j]);
            std::cout << "addres :" << m_ip_address[j] << std::endl;
            m_socketAress.sin_addr.s_addr = inet_addr(m_ip_address[j].c_str());
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
        m_port.clear();
        m_ip_address.clear();
        indexing.push_back(findexing);
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
    findexing.push_back(fd);
    if (fd < 0)
        exitWithError("Cannot create socket");
    if (setsockopt(fd, SOL_SOCKET, SO_NOSIGPIPE, (char *)&i, sizeof(i)) < 0)
        exitWithError("----------setsockopt-----------");
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&i, sizeof(i)) < 0)
        exitWithError("----------setsockopt-----------");
    if (bind(fd, (sockaddr *)&m_socketAress, m_socketAddress_len) < 0)
        exitWithError("Cannot connect socket to address");
    m_socket.push_back(fd);
    return 0;
}

int setNonblocking(int fd)
{
    return fcntl(fd, F_SETFL, O_NONBLOCK);
}

int http::TcpServer::acceptConnection(int fd)
{
    int new_socket;
    size_t c;

    for (c = 0; c < m_socket.size(); c++)
    {
        if (m_socket[c] == fd)
            break;
    }
    std::cout << "socket: " << m_socket[c] << std::endl;
    m_socketAress = class_m_socketAress[c];
    m_socketAddress_len = class_m_socketAddress_len[c];
    std::cout << inet_ntoa(m_socketAress.sin_addr) << std::endl;
    std::cout << ntohs(m_socketAress.sin_port) << std::endl;
    new_socket = accept(fd, (sockaddr *)&m_socketAress, (socklen_t *)&m_socketAddress_len);
    if (new_socket < 0)
    {
        std::ostringstream ss;
        ss << "Server failed to accept incoming connection from ADDRESS: "
           << inet_ntoa(class_m_socketAress[c].sin_addr) << "; PORT: "
           << ntohs(class_m_socketAress[c].sin_port);
        exitWithError(ss.str());
    }
    else
    {
        setNonblocking(new_socket);
        m_new_socket.push_back(new_socket);
    }
    return new_socket;
}

int http::TcpServer::listening()
{
    int max_fd;

    max_fd = 0;
    FD_ZERO(&readst);
    for (size_t i = 0; i < m_socket.size(); i++)
    {
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

void http::TcpServer::save(int fd, int client, int size)
{
    (void)fd;
    std::string s(buffer, size);
    // std::cout << "hello :" << fd << std::endl;
    if (clients[client].flag == 0 && clients[client].read_status == 0)
        clients[client].req = pars_request(&clients[client], s);
    else if (clients[client].flag == 1)
        clients[client].req->handle_body(&clients[client], s);
}

bool http::TcpServer::isMaster(int fd)
{
    for (size_t i = 0; i < m_socket.size(); i++)
        if (fd == m_socket[i])
            return true;
    return (false);
}
int http::TcpServer::findIndex(int fd)
{
    for (size_t i = 0; i < indexing.size(); i++)
    {
        for (size_t j = 0; j < indexing[i].size(); j++)
        {
            if (fd == indexing[i][j])
                return i;
        }
    }
    return (0);
}

void http::TcpServer::startListen(Parsed *data)
{
    int bytesReceived, max_fd, max_fd_tmp, act, max_fd_check;
    fd_set read_tmp, write_tmp;
    std::ostringstream ss;

    FD_ZERO(&read_tmp);
    FD_ZERO(&write_tmp);
    max_fd = listening();
    max_fd_tmp = max_fd;
    while (true)
    {
        read_tmp = readst;
        write_tmp = writest;
        log("====== Waiting for a new connection ======\n");
        act = select(max_fd + 1, &readst, &writest, NULL, NULL);
        if (act < 0)
            exitWithError("--------select error-------");
        for (int i = 0; i < max_fd + 1; i++)
        {
            if (FD_ISSET(i, &readst))
            {
                if (isMaster(i))
                {
                    int index = findIndex(i);
                    max_fd_check = acceptConnection(i);
                    std::cout << "fd in:" << max_fd_check << std::endl;
                    size_t cl = 0;
                    for (; cl < clients.size(); cl++)
                    {
                        if (clients[cl].client_fd == max_fd_check)
                        {
                            clients[cl].read_status = 0;
                            clients[cl].write_sened = 0;
                            clients[cl].chunked = 0;
                            clients[cl].client_body = "";
                            clients[cl].client_res_message = "";
                            clients[cl].readed = 0;
                            clients[cl].read_len = 0;
                            clients[cl].serverIndex = index;
                        }
                    }
                    if (cl >= clients.size()){
                        clients.push_back(client(0, 0, 0, index, max_fd_check));
                    }
                    FD_SET(max_fd_check, &read_tmp);
                    if (max_fd_check > max_fd_tmp)
                        max_fd_tmp = max_fd_check;
                }
                else if (FD_ISSET(i, &readst))
                {
                    bytesReceived = read(i, buffer, BUFFER_SIZE);
                    std::cout << buffer << std::endl;
                    if (bytesReceived > 0)
                    {
                        log("======   message request received   ======\n");
                        buffer[bytesReceived] = 0;
                        size_t cl1 = 0;
                        for (; cl1 <= clients.size(); cl1++)
                        {
                            if (clients[cl1].client_fd == i){
                                save(i, cl1, bytesReceived);
                                break;
                            }
                        }
                        if ((clients[cl1].read_status == 1 && clients[cl1].flag == 2) ||
                            (clients[cl1].read_len == clients[cl1].readed && clients[cl1].flag == 2))
                        {
                            FD_SET(i, &write_tmp);
                            FD_CLR(i, &read_tmp);
                            std::cout << i << std::endl;
                            buildResponse(data, i);
                        }
                    }
                    else
                    {
                        FD_CLR(i, &read_tmp);
                        close(i);
                    }
                }
            }
            else if (FD_ISSET(i, &writest))
            {
                int send = sendResponse(i);
                if (send >= 0)
                {
                    size_t cl2 = 0;
                    for (; cl2 < clients.size(); cl2++)
                    {
                        if (clients[cl2].client_fd == i){
                            clients[cl2].read_status = status;
                            break;
                        }
                    }
                    if (clients[cl2].write_sened == clients[cl2].write_len)
                    {
                        log("======   response messge sended   ======\n");
                        std::cout << clients[cl2].write_sened << "|" << clients[cl2].write_len << std::endl; 
                        clients[cl2].read_status = 0;
                        clients[cl2].flag = 0;
                        clients[cl2].write_len = 0;
                        clients[cl2].write_sened = 0;
                        clients[cl2].readed = 0;
                        clients[cl2].fd_enabeld = 0;
                        clients[cl2].read_status = 0;
                        clients[cl2].write_len = 0;
                        clients[cl2].write_sened = 0;
                        delete clients[cl2].req;
                        clients[cl2].req = 0;
                        remove(clients[cl2].client_resFile.c_str());
                        remove(clients[cl2].client_body.c_str());
                        std::cout << "sended :" << clients[cl2].write_len << std::endl;
                        FD_CLR(i, &write_tmp);
                        close(i);
                    }
                }
                else if (send == 0)
                {
                    FD_CLR(i, &write_tmp);
                    close(i);
                }
            }
        }
        max_fd = max_fd_tmp;
        readst = read_tmp;
        writest = write_tmp;
    }
}

int http::TcpServer::closeServer()
{
    for (std::vector<int>::iterator ite = m_socket.begin(); ite < m_socket.end(); ite++)
        close(*ite);
    exit(0);
}

void http::TcpServer::buildResponse(Parsed *data, int cl)
{
	std::ostringstream ss2;
	std::ofstream myfile1;


    size_t cl2;
    if (!data)
        return;
    for (cl2 = 0; cl2 < clients.size(); cl2++)
    {
        if (clients[cl2].client_fd == cl)
            break;
    }
    clients[cl2].req->body = clients[cl2].client_body;
    response res(clients[cl2].req, data->getDate()[clients[cl2].serverIndex]);

    // if (clients[cl2].req->method == "GET")
        m_serverMessage = res.get_response();
    // i++;
    // else if(data->req->method == "DELETE")
    //     ;
    // else if(data->req->method == "POST")
    //     ;
    // else {
    //     std::cerr << "Unsupported HTTP method: " << data->req->method << '\n';
    //     this->m_serverMessage = "HTTP/1.1 405 Method Not Allowed\r\n\r\n";
    //     return ;
    // }
	ss2 << "/tmp/Response_" << clients[cl2].client_fd;
    clients[cl2].client_resFile = ss2.str();
	myfile1.open(ss2.str());
    myfile1 << this->m_serverMessage;
    myfile1.close();
    clients[cl2].write_len = this->m_serverMessage.size();
    clients[cl2].write_sened = 0;
    this->m_serverMessage.resize(0);
}

int http::TcpServer::sendResponse(int fd)
{
    char wBuffer[BUFFER_SIZE];
    int count = 0;

    size_t cl2 = 0;
    for (; cl2 < clients.size(); cl2++)
        if (clients[cl2].client_fd == fd)
            break;
    std::ifstream MyResFile(clients[cl2].client_resFile);
    if (clients[cl2].write_sened > clients[cl2].write_len)
        return 0;
    MyResFile.seekg(clients[cl2].write_sened, std::ios::beg);
    MyResFile.read(wBuffer, BUFFER_SIZE);
    count = MyResFile.gcount();
    clients[cl2].write_sened += count;
    MyResFile.close();
    std::cout << "readed from the file" << count << std::endl;
    return (write(fd, wBuffer, count));
}