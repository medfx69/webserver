#include "../http_TcpServer.hpp"

// void   get_response(std::string request_method,int &new_socket, std::string request_path)
// {
//     if (request_method != "GET") {
//         std::cerr << "Unsupported HTTP method: " << request_method << '\n';
//         std::string response = "HTTP/1.1 405 Method Not Allowed\r\n\r\n";
//         send(new_socket, response.c_str(), response.size(), 0);
//         close(new_socket);
//         return ;
//     }

//     std::string file_path = "." + request_path;
//     std::ifstream file(file_path.c_str());
//     if (!file.is_open()) {
//         std::cerr << "Failed to open file: " << file_path << '\n';
//         std::string response = "HTTP/1.1 404 Not Found\r\n\r\n";
//         send(new_socket, response.c_str(), response.size(), 0);
//         close(new_socket);
//         return ;
//     }
//     std::ostringstream file_content;
//     file_content << file.rdbuf();
//     file.close();
//     std::string response = "HTTP/1.1 200 OK\r\n\r\n" + file_content.str();
//     send(new_socket, response.c_str(), response.size(), 0);
//     close(new_socket);
// }

