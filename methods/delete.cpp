#include "../http_TcpServer.hpp"
#include <sys/stat.h>
#include <stdbool.h>

bool file_exists(const char *filename)
{
    struct stat buffer;
    return stat(filename, &buffer) == 0 ? true : false;
}

int main()
{
    char *filename = "test";

    if (file_exists(filename))
    {
      std::cout << "FILE " << filename << " exists" << std::endl;
      if(!remove(filename))
        std::cout << "file exist 200 (OK)" << std::endl;
      else
        std::cout << "202 (Accepted, deletion is not a single operation)." << std::endl;
    }
    else
      std::cout << "FILE " << filename << " doesn't exists 204" << std::endl;
    return 0;
}