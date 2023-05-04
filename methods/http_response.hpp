#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <dirent.h>
#include "../parsing/includes.hpp"
#include "../http_TcpServer.hpp"

class response
{
    private:
    public:
        std::string	get_response(Parsed& data, server& config);
		std::string    CheckPathType(Parsed& data);

};

#endif