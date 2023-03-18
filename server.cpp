// #include "http_TcpServer.hpp"
#include <fcntl.h>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>

struct data{
    std::string block_name;
    struct data *block;
    std::vector<std::string> dirictive;
};

struct data *read_data(std::ifstream& myFile){
    std::string readeddata;
    struct data *confData = new data();

    while (std::getline(myFile, readeddata)){

        if (readeddata.find(';') != std::string::npos && readeddata.find('{') == std::string::npos)
            confData->dirictive.push_back(readeddata);
        else if (readeddata.find('{') != std::string::npos)
        {
            confData->block_name = readeddata.substr(0, readeddata.find("{"));
            confData->block = read_data(myFile);
        }
        else if (readeddata.find('}') != std::string::npos)
            break ;
        readeddata.empty();
    }
    return confData;
}

int main(int ac, char **av){
    if (ac == 2){
        struct data *confData;
        std::string file(av[1]);
        std::ifstream myFile(file);
        confData = read_data(myFile);
        struct data *confData2;
        confData2 = confData;
        while (1){
            for (std::vector<std::string>::iterator first = confData2->dirictive.begin(); confData2->dirictive.end() > first; first++)
                std::cout << *first << std::endl;
            
            if (confData2->block == nullptr)
                break;
            std::cout << ">>>>>>>" << confData2->block_name<< std:: endl;
            confData2 = confData2->block;
        }

        // http::TcpServer server = http::TcpServer("0.0.0.0", 8080);
        // server.startListen();
        return (0);

    }
}