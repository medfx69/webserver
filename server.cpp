#include "http_TcpServer.hpp"
#include "parsing/includes.hpp"


int main(int ac, char **av)
{
    if (ac == 2)
    {
        Parsed *pr = new Parsed(av[1]);
        signal(SIGPIPE, SIG_IGN);
        http::TcpServer server = http::TcpServer(pr);
        server.startListen(pr);
    }
    else if (ac == 1){
        char s[] = "default_conf";
        Parsed *pr = new Parsed(s);
        signal(SIGPIPE, SIG_IGN);
        http::TcpServer server = http::TcpServer(pr);
        server.startListen(pr);
    }
    return (0);
}