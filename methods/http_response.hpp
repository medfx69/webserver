#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include "../parsing/includes.hpp"
#include "../http_TcpServer.hpp"

class response
{
    private:
        // http::TcpServer *tcp;
    public:
        // response();
        std::string get_response(Parsed *data);
};

#endif