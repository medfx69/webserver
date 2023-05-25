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

bool response::DELETE(std::string path)
{
  char *realPath;
  std::string pathtype = checkPathType();


  std::cout <<">>>>>>>>>>>>>>>>>>>>"<< path << std::endl;
  realPath = realpath(path.c_str(), NULL);
  if (!realPath)
      return std::cout << "permission denied" << std::endl;
  std::string check(realPath);
  std::cout <<">>>>>>>>>>>>>>>>>>>>"<< check << std::endl;
  if (check.find(config->location[indexLocation].root) == std::string::npos)
      return std::cout << "permission denied" << std::endl;
	if(pathtype == "FOLDER")
  {
    del jj;
    DIR* dir = opendir(path.c_str());
    if (dir != NULL)
    {
      struct dirent* entry;
      while ((entry = readdir(dir)) != NULL)
      {
        if (entry->d_type == DT_REG)
        {
          jj.filename = path_creat(path, entry->d_name);
          std::cout << "File found: " << jj.filename << std::endl;
        }
        else if (entry->d_type == DT_DIR)
        {
          std::string fd = entry->d_name;
          f_remove(jj.filename);
          if(fd == "." || fd == "..")
            std::cout << entry->d_name << std::endl;
          else
          {
            jj.foldername = path_creat(path, entry->d_name);
            std::cout << "Subdirectory found: " << jj.foldername << std::endl;
            DELETE(jj.foldername);
            f_remove(jj.foldername);
          } 
        }
      }
      closedir(dir);
      // return 1;
    }
    else
      std::cout << "Folder not found or could not be opened" << std::endl;
  }
	else if(pathtype == "FILE")
    f_remove(path);
  return 0;
}
// int main()
// {
//   std::string fd = "folder";
//   if(DELETE(fd))
//     f_remove(fd);
// }



