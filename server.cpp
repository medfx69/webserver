// #include "http_TcpServer.hpp"
#include <fcntl.h>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>

struct server{
    std::string block_name;
    std::vector<struct server> block;
    std::vector<std::string> dir;
};

server read_block(std::ifstream &myFile, std::string block_start){
    int i;
    std::string readed;
    server s;

    s.block_name = block_start.substr(0, block_start.find('}'));
    while (getline(myFile, readed)){
        if (readed.find('}') != std::string::npos)
            return s;
        else if (readed.find(';'))
            s.dir.push_back(readed);
    }
    return s;
}

server parec(char *s){
    server ser;
    std::string file(s);
    std::ifstream myFile(file);
    std::string readed;

    while (getline(myFile, readed) && readed.find('{') == std::string::npos);
    ser.block_name = readed.substr(0, readed.find('{'));
    while (getline(myFile, readed)){
        if (readed.find('{') != std::string::npos)
            ser.block.push_back(read_block(myFile, readed));
        else if (readed.find(';') != std::string::npos)
            ser.dir.push_back(readed);
    }
    return ser;
}


int main(int ac, char **av){
    server sr;    
    sr = parec(av[1]);
    std::vector<server>::iterator it = sr.block.begin();
    while(it< sr.block.end()){
        std::vector<std::string>::iterator its;
        for (its = (*it).dir.begin(); its < (*it).dir.end(); its++){
        std::cout << (*it).block_name << ">> ";
            std::cout << *its << std::endl;

        }
        it++;
    }
    // http::TcpServer server = http::TcpServer("0.0.0.0", 8080);
    // server.startListen();
    return (0);

    // }
}