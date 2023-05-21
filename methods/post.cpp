#include "http_response.hpp"

std::string response::post()
{
    if(!config->location[indexLocation].upload.empty())
    {
        // uplaod the Post Request Body
        return errorPage(201);
    }
    // get_requested_resource()
    std::string pathtype = checkPathType();
    if(pathtype == "NOT FOUND")
        return errorPage(404);
    if(pathtype == "FILE")
    {
        // if_location_has_cgi()
        // {
            // run cgi on requested file with POST REQUEST_METHOD
            // Returtn Code Depend on cgi
        // }
        // else 
            // return status_code(403);
    }
    else if(pathtype == "FOLDER")
    {
            if(req->absoluteURI.back() != '/')
            {
                    // make a 301 redirection to request uri with “/” addeed at the end
                    return errorPage(301);
            }
            if(!config->location[indexLocation].index.empty())
            {
                req->absoluteURI += config->location[indexLocation].index[0];
                return getfile();
            }
            else
                return errorPage(403);
    }
    return errorPage(404);
}