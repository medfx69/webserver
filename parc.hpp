#ifndef PARC_HPP
#define PARC_HPP

#include <fcntl.h>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>

struct server_data{
    std::string block_name;
    std::vector<struct server_data> block;
    int level;
    std::vector<std::string> dir;
};

server_data read_block(std::ifstream &myFile, std::string block_start){
    int i;
    server_data s;
    std::string readed;


    s.block_name = block_start.substr(0, block_start.find('}'));
    while (getline(myFile, readed)){
        if (readed.find('}') != std::string::npos)
            return s;
        else if (readed.find(';') != std::string::npos)
            s.dir.push_back(readed);
    }
    return s;
}

server_data parec(char *s){
    server_data ser;
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


class Parsed
{
private:
    server_data s;
    
public:
    Parsed();
    Parsed &operator=(const Parsed & parsed);
    Parsed(const server_data &s);
    server_data getServer_data() const;
    Parsed(char *);
    ~Parsed();
};

Parsed::Parsed(char *file)
{
    s = parec(file);
}

Parsed &Parsed::operator=(const Parsed & parsed){
    server_data tmp;
    std::vector<struct server_data>::iterator it = parsed.getServer_data().block.begin();

    std::cout << "1 >>"<<( *parsed.getServer_data().block.begin()).block_name << std::endl;
    std::cout << "2 >>"<<(*it).block_name << std::endl;
    while(it < parsed.getServer_data().block.end()){
        s.block.push_back(*it);
        it++;
    }
    while (1);
    
    return *this;
}
Parsed::Parsed(const server_data &s)
{
    this->s = s;
}
Parsed::Parsed(){

}

server_data Parsed::getServer_data() const{
    return s;
}
Parsed::~Parsed()
{
}

#endif