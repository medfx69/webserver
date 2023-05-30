#include "http_response.hpp"

std::string generateRandomString(int length) {
    std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string randomString;

    for (int i = 0; i < length; ++i) {
        int randomIndex = rand() % charset.length();
        randomString += charset[randomIndex];
    }
    return randomString;
}

void	response::uploadbody()
{
	std::string filename = req->absoluteURI + "/";
	if(req->boundry.empty())
	{
		filename += generateRandomString(8);
		std::cout << "filename: " << filename << " =========================="<< std::endl;
		std::map<std::string, std::string>::iterator it = req->data.find("Content-Type:");
		if(it != req->data.end())
		{
			if (this->mimeTypeMap2.find((*it).second) != this->mimeTypeMap2.end())
				filename += "." + (*this->mimeTypeMap2.find((*it).second)).second;
		}
	}
	else
	{
		std::map<std::string, std::string>::iterator it = boundray.find("Content-Disposition");
		std::map<std::string, std::string>::iterator it2 = boundray.find("Content-Type");
		if(it != boundray.end() && it2 != boundray.end())
		{
			if ((*it).second.find("filename") != std::string::npos)
			{
				filename +=	(*it).second.erase(0, (*it).second.find("filename=") + 10);
				filename.erase(filename.size() - 1, filename.size());
			}
			else if (it2 != boundray.end())
        	{
				filename += generateRandomString(8);
				if (this->mimeTypeMap2.find((*it2).second) != this->mimeTypeMap2.end())
					filename += "." + (*this->mimeTypeMap2.find((*it).second)).second;
			}
		}
		else if (it2 != boundray.end())
        {
			filename += generateRandomString(8);
			if (this->mimeTypeMap2.find((*it2).second) != this->mimeTypeMap2.end())
				filename += "." + (*this->mimeTypeMap2.find((*it).second)).second;
		}
	}
	std::ofstream file_content(filename);
	file_content << Bbody;
	file_content.close();
}

int	response::addboundaryheader(std::string &file)
{
	std::string line;
	int length = 0;
	line = file.substr(0, file.find("\r\n"));
	while(line.find(':') != std::string::npos)
	{
        std::string key = line.substr(0, line.find(":"));
        std::string value = line.substr(line.find(":") + 1, line.size());
        boundray[key] = value;
		length += line.size();
		file.erase(0, file.find("\r\n") + 2);
		line = file.substr(0, file.find("\r\n"));
    }
	file.erase(0, 2);
	return length;
}

std::string response::cgi__()
{
	if(config->location[indexLocation].cgi_path.empty())
		return "";
	std::string pathtype = checkPathType();
	if(pathtype == "FILE")
	{
		std::string extension = getFileExtension();
		if(config->location[indexLocation].cgi_path == extension)
		{
			if (extension == "py")
			{
				std::pair<std::string, std::string> p("Program_Name:", "./cgi/python-cgi");
				req->data.insert(p);
			}
			else if (extension == "php"){
				std::cout << "php\n";
				std::pair<std::string, std::string> p("Program_Name:","./cgi/php-cgi");
				req->data.insert(p);
			}
			return exec_outfile(req->client_reqFile, req->data);
		}
	}
	else if(pathtype == "FOLDER")
	{
		if (!config->location[indexLocation].index.empty())
		{
			std::string pathfile;
			for(size_t i = 0; i < config->location[indexLocation].index.size(); i++)
			{
				pathfile = req->absoluteURI + config->location[indexLocation].index[i];
    	        pathfile = cleanupURI(pathfile);
				std::ifstream file(pathfile);
				if(file.is_open() )
				{
					file.close();
					std::string saveuri = req->absoluteURI;
					req->absoluteURI = pathfile;
					std::string extension = getFileExtension();
					if(config->location[indexLocation].cgi_path == extension)
   					{
						std::cout << "extension====================2 " << extension << std::endl;
						if (extension == "py"){
							std::pair<std::string, std::string> p("Program_Name:", "./cgi/python-cgi");
							req->data.insert(p);
						}
						else if (extension == "php"){
							std::cout << "php\n";
							std::pair<std::string, std::string> p("Program_Name:","./cgi/php-cgi");
							req->data.insert(p);

						}
    				    return exec_outfile(req->client_reqFile, req->data);
    				}
					req->absoluteURI = saveuri;
				}
			}
		}
		// return generateResponse(403);
	}
	return "";
}


std::string response::POST()
{
	if(!methode_allowded("POST"))
		return generateResponse(405);
	std::string c = cgi__();
	if(!c.empty())
		return c;
	if(config->location[indexLocation].upload.empty())
			return generateResponse(404);
	req->absoluteURI = config->location[indexLocation].root + config->location[indexLocation].upload;
	req->absoluteURI = cleanupURI(req->absoluteURI);
	std::cout << "UPLOAD: " << req->absoluteURI << std::endl;
	if(!req->boundry.empty())
	{
		bodyfile.open(req->body);
    	if (!bodyfile)
			return generateResponse(404);
		std::ostringstream bodycontent;
		bodycontent << bodyfile.rdbuf();
		std::string body = bodycontent.str();
		std::string line;
		while(body.size()){
			if (body.substr(0, body.find("\r\n")) == "--" + req->boundry + "--")
				break;
			line = body.substr(0, body.find("\r\n"));
			body = body.erase(0, body.find("\r\n") + 2);
			addboundaryheader(body);
  			Bbody = body.substr(0, body.find("--" + req->boundry) - 2);
			uploadbody();
			body.erase(0, body.find("--" + req->boundry));
		}
	}
	else
	{
		std::ifstream bodyfile(req->body);
		std::ostringstream bodycontent;
		bodycontent << bodyfile.rdbuf();
		Bbody = bodycontent.str();
		bodyfile.close();
		uploadbody();
	}
	return generateResponse(201);
}