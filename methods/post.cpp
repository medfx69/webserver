#include "http_response.hpp"

std::string response::post()
{
    // if(req->chunked_transfer_encoding.empty() || req->content-lenght.empty())
        // return status_code(400);
    // if(!config->location[index].upload.empty())
    // {
        // uplaod the Post Request Body
        // return status_code(201);
    // }
    // get_requested_resource()
    // if(not found)
        // return status_code(404);
    // get_resource_type()
    // if(file)
    // {
        // if_location_has_cgi()
        // {
            // run cgi on requested file with POST REQUEST_METHOD
            // Returtn Code Depend on cgi
        // }
        // else 
            // return status_code(403);
    // }
    // else if(dir)
    // {
            // if(request uri doesn’t have “/” at the end)
            // {
                    // make a 301 redirection to request uri with “/” addeed at the end
                    // return status_code(301);
            // }
            // if( dir has index files)
                    // return getfile()
            // else if(directory does’t have an index file)
                    // return status_code(403);
    // }
    return "";
}