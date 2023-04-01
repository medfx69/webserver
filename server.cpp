#include "http_TcpServer.hpp"
#include "parc.hpp"

int main(int ac, char **av)
{
    Parsed pr(av[1]);
    Parsed *prq = &pr;

    (void)ac;
    // std::cout << (*pr.getserver()->block.begin()).block_name << std::endl;
    // (*pr.getserver()->block.begin()).block_name = "hello";
    // std::cout << ">>> " << (*pr.getserver()->block.begin()).block_name << std::endl;
    // std::cout << (*prq.getserver()->block.begin()).block_name << std::endl;
    // std::cout << *(prq.getserver()->dir.end() - 1) << std::endl;
    // std::cout << (*pr.getserver()->block.begin()).block_name << std::endl;
    http::TcpServer server = http::TcpServer(prq->getDate()->listen.first, std::stoi(prq->getDate()->listen.second));
    server.startListen(prq);
    return (0);

    // }
}