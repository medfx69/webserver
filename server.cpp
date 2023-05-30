#include "http_TcpServer.hpp"
#include "parsing/includes.hpp"


int main(int ac, char **av)
{
    if (ac == 2)
    {
        Parsed *pr = new Parsed(av[1]);
        signal(SIGPIPE, SIG_IGN);
        // std::cout << (*pr.getserver()->block.begin()).block_name << std::endl;
        // (*pr.getserver()->block.begin()).block_name = "hello";
        // std::cout << ">>> " << (*pr.getserver()->block.begin()).block_name << std::endl;
        // std::cout << (*prq.getserver()->block.begin()).block_name << std::endl;
        // std::cout << *(prq.getserver()->dir.end() - 1) << std::endl;
        // std::cout << (*pr.getserver()->block.begin()).block_name << std::endl;
        http::TcpServer server = http::TcpServer(pr);
        // std::cout << ">>>>>" << pr.getDate()[0].root << std::endl;
        server.startListen(pr);
    }
    return (0);

    // }
}