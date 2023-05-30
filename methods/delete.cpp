#include "http_response.hpp"

bool hasWritePermission(const std::string& path)
{
    struct stat fileInfo;
    if (stat(path.c_str(), &fileInfo) == 0)
	{
        mode_t mode = fileInfo.st_mode;
        return (mode & S_IWUSR) != 0;
    }
    return false;
}

bool response::checkPermission(std::string path, std::string type)
{
	if(type == "FOLDER")
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
					jj.filename = path + "/" + entry->d_name;
					if(!hasWritePermission(jj.filename))
						return 0;
				}
				else if (entry->d_type == DT_DIR)
				{
					std::string fd = entry->d_name;
					if(fd != "." && fd != "..")
					{
						jj.foldername = path + "/" + entry->d_name;
						if(!hasWritePermission(jj.foldername) || !checkPermission(jj.foldername, "FOLDER"))
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
	else if(type == "FILE" && !hasWritePermission(path.c_str()))
			return 0;
	return 1;
}

bool response::fd_remove(std::string path)
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
				jj.filename = path + "/" + entry->d_name;
				remove(jj.filename.c_str());
			}
			else if (entry->d_type == DT_DIR)
			{
				std::string fd = entry->d_name;
				if(fd != "." && fd != "..")
				{
					jj.foldername = path + "/" + entry->d_name;
					fd_remove(jj.foldername);
					remove(jj.foldername.c_str());
				}
			}
	  	}
		closedir(dir);
		remove(path.c_str());
		return 1;
	}
	else
		return 0;
	return 1;
}

std::string response::DELETE()
{
	if(!methode_allowded("DELETE"))
		return generateResponse(405);
	std::string type = checkPathType();
	if(type == "NOT FOUND")
		return generateResponse(404);
	else if(type == "FOLDER" && req->absoluteURI.back() != '/')
		return generateResponse(409);
	else if(!checkPermission(req->absoluteURI, type))
		return generateResponse(403);
	if(type == "FILE")
		remove(req->absoluteURI.c_str());
	else if(type == "FOLDER")
		fd_remove(req->absoluteURI);
	return generateResponseHeader("text/html", "0", 204);
}