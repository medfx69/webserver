#include "http_response.hpp"

// std::string   response::get_response(Parsed *data)
// {
    // std::string     m_serverMessage;
// 
    // std::string file_path = "." + data->req->absoluteURI;
    // std::ifstream file(file_path.c_str());
    // if (!file.is_open()) {
        // std::cerr << "Failed to open file: " << file_path << '\n';
        // m_serverMessage = "HTTP/1.1 404 Not Found\r\n\r\n";
        // return m_serverMessage;
    // }
    // std::ostringstream file_content;
    // file_content << file.rdbuf();
    // file.close();
    // m_serverMessage = "HTTP/1.1 200 OK\r\n\r\n" + file_content.str();
    // return m_serverMessage;
// }

std::string   response::get_response(Parsed *data)
{
    std::string     m_serverMessage;
    std::string pathtype = CheckPathType(data->req->absoluteURI);
    if(pathtype == "FILE")
    {
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
}

std::string    CheckPathType(const std::string& path)
{
    DIR* dir = opendir(path.c_str());
    if(dir == NULL)
        return "NOT FOUND";
    struct dirent *entry;
    if((entry = readdir(dir)) != NULL)
    {
        if(entry->d_type == DT_REG)
            return "FILE";
        else if(entry->d_type == DT_DIR)
            return "FOLDER";
        else
        {
            std::cout << "Error CheckPathType function\n";
            return "NOT FOUND";
    }
}

//1 get resource type (dir or file)
// if dir : 
//          if uri has '/' in end