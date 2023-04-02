#include "../http_TcpServer.hpp"
#include <sys/stat.h>
#include <stdbool.h>
#include <iostream>
#include <cstring>
#include <dirent.h>

bool folder_exists(const char *folder_path)
{
  DIR* dir = opendir(folder_path);
  if (dir != NULL)
  {
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL)
    {
      if (entry->d_type == DT_REG)
      {
        std::cout << "File found: " << entry->d_name << std::endl;
      }
      else if (entry->d_type == DT_DIR)
        std::cout << "Subdirectory found: " << entry->d_name << std::endl;
    }
    closedir(dir);
  }
  else
      std::cout << "Folder not found or could not be opened" << std::endl;
  return 0;
}

bool file_exists(const char *filename)
{
  struct stat buffer;
  return stat(filename, &buffer) == 0 ? true : false;
}

int main()
{
  char *filename = "test.jj";
  folder_exists(filename);
  return (0);
  if (file_exists(filename))
  {
    std::cout << "FILE " << filename << " exists" << std::endl;
    if(!remove(filename))
      std::cout << "204 No Content status code." << std::endl;
  }
  else
    std::cout << "FILE " << filename << "404 Not Found status code" << std::endl;
  return 0;
}

// an example of a DELETE request in HTTP:
// DELETE /resource HTTP/1.1
// Host: example.com
// Authorization: Basic YWxhZGRpbjpvcGVuc2VzYW1l

// If the client is not authorized to delete the resource,
// the server should respond with a 403 Forbidden status code


