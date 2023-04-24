#include "http_response.hpp"

void   response::get_response()
{
    std::string file_path = "." + this->tcp->_data->req->absoluteURI;
    std::ifstream file(file_path.c_str());
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << file_path << '\n';
        this->tcp->m_serverMessage = "HTTP/1.1 404 Not Found\r\n\r\n";
        return ;
    }
    std::ostringstream file_content;
    file_content << file.rdbuf();
    file.close();
    this->tcp->m_serverMessage = "HTTP/1.1 200 OK\r\n\r\n" + file_content.str();
}