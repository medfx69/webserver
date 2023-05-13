#include "../http_TcpServer.hpp"
#include "methods.hpp"

#include <sys/stat.h>
#include <stdbool.h>
#include <iostream>
#include <cstring>
#include <dirent.h>
#include "http_response.hpp"

void  response::f_remove(std::string path)
{
  if(remove(path.c_str()))
    std::cout << "204 No Content status code. " << path << std::endl;
  else
    std::cout << path << " deleted" << std::endl;
}

std::string response::path_creat(std::string path, std::string join_path)
{
  return path += "/" + join_path;
}

bool response::folder_exists(std::string folder_path)
{
  DIR* dir = opendir(folder_path.c_str());
  del jj;
  if (dir != NULL)
  {
    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL)
    {
      if (entry->d_type == DT_REG)
      {
        jj.filename = path_creat(folder_path, entry->d_name);
        std::cout << "File found: " << jj.filename << std::endl;
        f_remove(jj.filename);
      }
      else if (entry->d_type == DT_DIR)
      {
        std::string fd = entry->d_name;
        if(fd == "." || fd == "..")
          std::cout << entry->d_name << std::endl;
        else
        {
          jj.foldername = path_creat(folder_path, entry->d_name);
          std::cout << "Subdirectory found: " << jj.foldername << std::endl;
          folder_exists(jj.foldername);
          f_remove(jj.foldername);
        } 
      }
    }
    closedir(dir);
    return 1;
  }
  else
      std::cout << "Folder not found or could not be opened" << std::endl;
  return 0;
}

// int main()
// {
//   std::string fd = "folder";
//   if(folder_exists(fd))
//     f_remove(fd);
// }



