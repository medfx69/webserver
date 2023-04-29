#include "http_response.hpp"

std::string   response::get_response(Parsed *data)
{
    std::string     m_serverMessage;


    std::string file_path = "." + data->req->absoluteURI;
    std::ifstream file(file_path.c_str());
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << file_path << '\n';
        m_serverMessage = "HTTP/1.1 404 Not Found\r\n\r\n";
        return m_serverMessage;
    }
    std::ostringstream file_content;
    file_content << file.rdbuf();
    file.close();
    m_serverMessage = "HTTP/1.1 200 OK\r\n\r\n" + file_content.str();
    return m_serverMessage;
}