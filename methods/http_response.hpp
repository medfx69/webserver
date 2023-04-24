#ifndef HTTP_RESPONSE.HPP
#define HTTP_RESPONSE.HPP

#include "../parsing/includes.hpp"
#include "../http_TcpServer.hpp"

class response
{
    private:
        http::TcpServer *tcp;
    public:
        // response();
        void get_response();
};

#endif