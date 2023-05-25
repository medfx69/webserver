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

bool hasWritePermission(const std::string& path) {
    struct stat fileInfo;
    if (stat(path.c_str(), &fileInfo) == 0) {
        mode_t mode = fileInfo.st_mode;
        // Check if the user has write permission
        return (mode & S_IWUSR) != 0;
    }
    return false;
}

bool response::checkPermission(std::string path, std::string i)
{
	// struct stat st;
	if(i == "FOLDER")
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
					if(!hasWritePermission(jj.filename))
						return 0;
				}
				else if (entry->d_type == DT_DIR)
				{
					std::string fd = entry->d_name;
					if(fd == "." || fd == "..")
					  std::cout << entry->d_name << std::endl;
					else
					{
						jj.foldername = path_creat(path, entry->d_name);
						std::cout << "Subdirectory found: " << jj.foldername << std::endl;
						if(!hasWritePermission(jj.foldername))
							return 0;
						if(!checkPermission(jj.foldername, "FOLDER"))
							return 0;
					} 
				}
		  	}
			closedir(dir);
			return 1;
		}
		else
			return 0;
	}
	else if(i == "FILE")
	{
		if(!hasWritePermission(path.c_str()))
			return 0;
	}
	else
		return 0;
	return 1;
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
					f_remove(jj.filename);
				}
				else if (entry->d_type == DT_DIR)
				{
					std::string fd = entry->d_name;
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
			f_remove(path);
			return 1;
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
