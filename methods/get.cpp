#include "http_response.hpp"

std::string    response::checkPathType()
{
	struct stat st;
	if(stat(req->absoluteURI.c_str(), &st) != 0)
		return "NOT FOUND";
	else if(S_ISDIR(st.st_mode))
		return "FOLDER";
	else
		return "FILE";
}

std::string response::createIndexHtml()
{
	DIR* dir = opendir(req->absoluteURI.c_str());
	if(dir != NULL)
	{
		std::ostringstream htmlfile;
		htmlfile << "<!DOCTYPE html>\n<html lang=\"en\">\n\t\t<body>\n";
		struct dirent* entry;
		while((entry = readdir(dir)) != NULL)
		{
			std::string fd = entry->d_name;
			if(fd != "." && fd != "..")
			{
				if (entry->d_type == DT_DIR)
					htmlfile << "\t\t\t<a href='" << fd << "'/>" << fd << "/</a><br /><br />" << std::endl;
				else
					htmlfile << "\t\t\t<a href='" << fd << "'>" << fd << "</a><br /><br />" << std::endl;

			}
		}
		closedir(dir);
		htmlfile << "\t\t</body>\n</html>";
		content_lenght = htmlfile.str().size();
		return generateResponseHeader("text/html", std::to_string(status.size()), 200) + htmlfile.str();
	}
    return generateResponse(404);
}

std::string response::getfile()
{
    std::string extension = getFileExtension();
	std::cout << "extension==================== " << extension << std::endl;
    if(!config->location[indexLocation].cgi_path.empty()
        && config->location[indexLocation].cgi_path == extension)
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
	std::ifstream file(req->absoluteURI);
	if (!file.is_open())
    {
		std::cerr << "Failed to open file: " << req->absoluteURI << '\n';
		return generateResponse(404);
	}
	std::ostringstream file_content;
	file_content << file.rdbuf();
	file.close();
	return generateResponseHeader(contentType(), std::to_string(file_content.str().size()), 200) + file_content.str();
}

std::string response::getfolder()
{
	if(req->absoluteURI.back() != '/')
	{
		req->absoluteURI += "/";
		status = "301";
	}
	if (!config->location[indexLocation].index.empty())
	{
		std::string pathfile;
		for(size_t i = 0; i < config->location[indexLocation].index.size(); i++)
		{
			pathfile = req->absoluteURI + config->location[indexLocation].index[i];
            pathfile = cleanupURI(pathfile);
			std::ifstream file(pathfile);
			if(file.is_open())
			{
				file.close();
				req->absoluteURI = pathfile;
				status = "200";
				return getfile();
			}
		}
	}
	else if(config->location[indexLocation].autoindex == "ON")
		return createIndexHtml();
	else if(config->location[indexLocation].autoindex == "OFF" )
		return generateResponse(403);
	return generateResponse(404);
}

std::string response::GET()
{
	if(!methode_allowded("GET"))
		return generateResponse(405);
	std::string pathtype = checkPathType();
	if(pathtype == "FILE")
		return getfile();
	else if(pathtype == "FOLDER")
		return getfolder();
	return generateResponse(404);
}