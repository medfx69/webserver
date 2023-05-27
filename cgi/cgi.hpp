#ifndef CGI_HPP
#define CGI_HPP

#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>

void setVaribels(std::map<std::string, std::string> reqHeader);
void exec(std::map<std::string, std::string> reqHeader);
int exec_outfile(std::string inFile, std::map<std::string, std::string> reqHeader);

#endif