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
				filename +=	(*it).second.erase(0, (*it).second.find("filename=") + 10);
			filename.erase(filename.size() - 1, filename.size());
		}
		else if (it != boundray.end())
        {
			if ((*it).second.find("name") != std::string::npos)
				filename +=	(*it).second.erase(0, (*it).second.find("name=") + 6);
			filename.erase(filename.size() - 1, filename.size());
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

std::string response::post()
{
	req->absoluteURI = "/upload";
	req->absoluteURI = matchLocation();
	if(!methode_allowded("POST"))
		return errorPage(405);
	if(!config->location[indexLocation].upload.empty())
    {
		if(!req->boundry.empty())
		{
			bodyfile.open(req->body);
    		if (!bodyfile)
			{
    			std::cerr << "Failed to open body file: " << req->body << std::endl;
    			exit(0);
			}
			std::ostringstream bodycontent;
			bodycontent << bodyfile.rdbuf();
			std::string body = bodycontent.str();
			// bodyfile.clear();
    		// bodyfile.seekg(0);
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
        return generateResponseHeader("text/html",status_code(201), 201);
    }
	return errorPage(404);
}



// get_requested_resource()
    	// std::string pathtype = checkPathType();
    	// if(pathtype == "NOT FOUND")
    	    // return errorPage(404);
    	// if(pathtype == "FILE")
    	// {
    	    // if_location_has_cgi()
    	    // {
    	        // run cgi on requested file with POST REQUEST_METHOD
    	        // Returtn Code Depend on cgi
    	    // }
    	    // else 
    	        // return status_code(403);
    	// }
    	// else if(pathtype == "FOLDER")
    	// {
    	//         if(req->absoluteURI.back() != '/')
    	//         {
    	//                 // make a 301 redirection to request uri with “/” addeed at the end
    	//                 return errorPage(301);
    	//         }
    	//         if(!config->location[indexLocation].index.empty())
    	//         {
    	//             req->absoluteURI += config->location[indexLocation].index[0];
    	//             return getfile();
    	//         }
    	//         else
    	//             return errorPage(403);
    	// }

	// }
	// else if(req->method == "DELETE")
	// {
	// 	DELETE(req->absoluteURI);
	// }