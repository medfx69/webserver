#include "http_response.hpp"
#include <sys/stat.h>

std::string    CheckPathType(Parsed& data)
{
    struct stat st;
    if(stat(data.req->absoluteURI.c_str(), &st) != 0)
        return "NOT FOUND";
    else if(S_ISDIR(st.st_mode))
        return "FOLDER";
    else
        return "FILE";
}

std::string getfile(std::string& pathfile)
{
    std::ifstream file(pathfile);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << pathfile << '\n';
        return "HTTP/1.1 404 Not Found\r\n\r\n";
    }
    std::ostringstream file_content;
    file_content << file.rdbuf();
    file.close();
    return "HTTP/1.1 200 OK\r\n\r\n" + file_content.str();
}

std::string getfolder(Parsed& data, server& config)
{
    if(data.req->absoluteURI.back() != '/')
        return "HTTP/1.1 301 Moved Permanently\r\n\r\n";
    if (!config.index.empty())
        return getfile(data.req->absoluteURI + "/" + config.index[0]);
    else if(config.autoindex == "OFF")
        return "HTTP/1.1 301 Moved Permanently\r\n\r\n";
    else if(config.autoindex == "ON")
    {
        DIR* dir = opendir(data.req->absoluteURI.c_str());
        if(dir != NULL)
        {
            struct dirent* entry;
            while((entry = readdir(dir)) != NULL)
            {
                if (entry->d_type == DT_REG)
                {
                    std::string file = entry->d_name;
                    if(file == config.index[0])
                        return getfile(file);
                }
            }
            closedir(dir);
        }
    }
}

std::string   response::get_response(Parsed& data, server& config)
{
    std::string     m_serverMessage;
    std::string pathtype = CheckPathType(data);
    if(pathtype == "FILE")
        return getfile(data.req->absoluteURI);
    else if(pathtype == "FOLDER")
        return getfolder(data, config);
    else {
		m_serverMessage = "HTTP/1.1 404 Not Found\r\n\r\n";
    }
    return m_serverMessage;
}

