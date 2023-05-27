#ifndef CGI_HPP
#define CGI_HPP

#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "../methods/http_response.hpp"
#include <vector>
#include <map>
#include <utility>

char **setVaribels(std::map<std::string, std::string> reqHeader);
void exec(std::map<std::string, std::string> reqHeader);

#endif