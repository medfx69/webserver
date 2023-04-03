#include "../http_TcpServer.hpp"
#include "methods.hpp"

#include <sys/stat.h>
#include <stdbool.h>
#include <iostream>
#include <cstring>
#include <dirent.h>

void  file_remove(const char *filename)
{
  if(!remove(filename))
    std::cout << "204 No Content status code." << std::endl;
}

bool folder_exists(const char *folder_path)
{
  DIR* dir = opendir(folder_path);
  del jj;
  if (dir != NULL)
  {
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL)
    {
      if (entry->d_type == DT_REG)
      {
        jj.filename = folder_path;
        jj.filename += "/";
        jj.filename += entry->d_name;
        file_remove(jj.filename.c_str());
        std::cout << "File found: " << jj.filename << std::endl;
      }
      else if (entry->d_type == DT_DIR)
      {
        std::string fd = entry->d_name;
        if(fd == "." || fd == "..")
          std::cout << entry->d_name << std::endl;
        else
        {
          jj.foldername= folder_path;
          jj.foldername += "/";
          jj.foldername += entry->d_name;
          std::cout << "Subdirectory found: " << jj.foldername << std::endl;
          folder_exists(jj.foldername.c_str());
          file_remove(jj.foldername.c_str());
        } 
      }
      file_remove(folder_path);
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
  std::string filename = "folder";
  folder_exists(filename.c_str());
  return (0);
  if (file_exists(filename.c_str()))
  {
    std::cout << "FILE " << filename << " exists" << std::endl;
    if(!remove(filename.c_str()))
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


