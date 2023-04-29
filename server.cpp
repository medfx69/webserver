#include "http_TcpServer.hpp"
#include "parsing/includes.hpp"


int main(int ac, char **av)
{
    if (ac == 2)
    {
        Parsed pr(av[1]);
        // Parsed *prq = &pr;

        (void)ac;
        {
           for (std::vector<server>::iterator it = pr.getDate().begin(); it < pr.getDate().end();it++)
           {
               std::cout << "---------- server name : " << (*it).server_name << std::endl;
               std::cout << "---------- data -------------------" << std::endl;
               std::cout << (*it).listen.first << "      " << (*it).listen.second << std::endl;
               std::cout << (*it).root << "     " <<  (*it).client_max_body_size << std::endl;
               std::cout << "---------- location -----------------\n\n" << std::endl;
               for (std::vector<Location>::iterator it2 = (*it).location.begin(); it2 < (*it).location.end();it2++)
               {
                //    std::cout << "---------" << std::endl;
               }
           }
        }
        {
            // for (std::vector<data_reader>::iterator it = pr.getserver().begin(); it < pr.getserver().end();it++)
            // {
            //     std::cout << "---------- server name : " << (*it).block_name << std::endl;
            //     std::cout << "---------- dirs -------------------" << std::endl;
            //     for (std::vector<data_reader>::iterator it2 = (*it).block.begin(); it2 < (*it).block.end();it2++)
		    //     {
            //         std::cout << "dflk    " << (*it).block_name << std::endl;
		    //     	for (std::vector<std::string>::iterator it4 = (*it2).dir.begin(); it4 < (*it2).dir.end();it4++)
		    //         {
            //             std::cout << (*it4) << std::endl;
		    //         }
		    //     }
            //     // std::cout << (*it). << std::endl;
            //     // std::cout << (*it).root << "     " <<  (*it).client_max_body_size << std::endl;
            //     std::cout << "\n\n---------- location -----------------\n\n" << std::endl;
            // }
        }

        // http::TcpServer server = http::TcpServer(prq->getDate()->listen.first, std::stoi(prq->getDate()->listen.second));
        // server.startListen(prq);
    }
    return (0);

    // }
}