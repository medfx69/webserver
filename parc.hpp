#ifndef PARC_HPP
#define PARC_HPP

#include <fcntl.h>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>

// struct location
// {
    
// };


// struct server{
//     std::vector<struct location> location;
//     std::string listen;
//     std::string server_name;
//     std::vector<std::pair<short, std::string>> error_page;
//     std::string client_max_body_size;
//     std::vector<std::string> index;
//     std::string autoindex;
//     std::string chunked_transfer_encoding;
// };

struct data_reader
{
    std::string block_name;
    std::vector<std::string> dir;
    std::vector<std::string> others;
    std::vector<struct data_reader> block;
};


data_reader read_block(std::ifstream &myFile, std::string block_start){
    int i;
    data_reader s;
    std::string readed;


    s.block_name = block_start.substr(0, block_start.find('}'));
    while (getline(myFile, readed)){
        if (readed.find('}') != std::string::npos)
            return s;
        else if (readed.find(';') != std::string::npos)
            s.dir.push_back(readed);
        else
            s.others.push_back(readed);
    }
    return s;
}



data_reader *parec(char *s){
    data_reader *ser;
    std::string file(s);
    std::ifstream myFile(file);
    std::string readed;

    ser = new data_reader();
    while (getline(myFile, readed) && readed.find('{') == std::string::npos);
    ser->block_name = readed.substr(0, readed.find('{'));
    while (getline(myFile, readed)){
        if (readed.find('{') != std::string::npos)
            ser->block.push_back(read_block(myFile, readed));
        else if (readed.find(';') != std::string::npos)
            ser->dir.push_back(readed);
        else
            ser->others.push_back(readed);
    }
    return ser;
}


class Parsed
{
    private:
        data_reader *s;
        // server data_handler;

    public:
        Parsed();
        Parsed &operator=(const Parsed & parsed);
        Parsed(const Parsed &s);
        data_reader *getserver() const;
        Parsed(char *);
        ~Parsed();
};

Parsed::Parsed(char *file)
{
    s = parec(file);
}

Parsed &Parsed::operator=(const Parsed & parsed){
    data_reader tmp;
    std::vector<struct data_reader>::iterator it = parsed.getserver()->block.begin();

    delete s;
    s = new data_reader();
    while (it <  parsed.getserver()->block.end()){
        s->block.push_back(*it);
        it++;
    }
    s->block_name = parsed.getserver()->block_name;
    std::vector<std::string>::iterator it2;
    for (it2 = parsed.getserver()->dir.begin(); it2 < parsed.getserver()->dir.end(); it2++){
        s->dir.push_back(*it2);
    }
    for (it2 = parsed.getserver()->others.begin(); it2 < parsed.getserver()->others.end(); it2++){
        s->others.push_back(*it2);
    }
    return *this;
}
Parsed::Parsed(const Parsed &s)
{
    *this = s;
}
Parsed::Parsed(){

    s = new data_reader();
}

data_reader *Parsed::getserver() const{
    return s;
}
Parsed::~Parsed()
{
    delete s;
}

#endif