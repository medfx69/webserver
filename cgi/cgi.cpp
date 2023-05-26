#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>

char **setVaribels(std::map<std::string, std::string> reqHeader){
    if (reqHeader.find("Content-Type:") == reqHeader.end())
    {
        std::pair<std::string, std::string> p ("Content-Type:", "");
        reqHeader.insert(p);
    }
    if (reqHeader.find("Content-Lengh:") == reqHeader.end())
    {
        std::pair<std::string, std::string> p ("Content-Lengh:", "0");
        reqHeader.insert(p);
    }
    if (reqHeader.find("Content-Length") == reqHeader.end())
    {
        std::pair<std::string, std::string> p ("Content-Length", "");
        reqHeader.insert(p);
    }
    if (reqHeader.find("Set-Cookie:") == reqHeader.end())
    {
        std::pair<std::string, std::string> p ("Set-Cookie:", "");
        reqHeader.insert(p);
    }
    if (reqHeader.find("Request-Method:") == reqHeader.end())
    {
        std::pair<std::string, std::string> p ("Request-Method:", "");
        reqHeader.insert(p);
    }
    if (reqHeader.find("Query-String:") == reqHeader.end())
    {
        std::pair<std::string, std::string> p ("Query-String:", "");
        reqHeader.insert(p);
    }
    char *s[] = {const_cast<char*>(("CONTENT_TYPE: " + (*reqHeader.find("Content-Type:")).second).c_str()),
                    const_cast<char*>(("REQUEST_METHOD: " + (*reqHeader.find("Request_Method:")).second).c_str()),
                    const_cast<char*>(("CONTENT_LENGTH: " + (*reqHeader.find("Content-Lengh:")).second).c_str()),
                    const_cast<char*>(("QUERY_STRING: " + (*reqHeader.find("Query-String:")).second).c_str()),
                    const_cast<char*>(("HTTP_COOKIE: " + (*reqHeader.find("Set-Cookie:")).second).c_str())
                    };
    return s;
}

void    exec(std::map<std::string, std::string> reqHeader)
{
    char* argv[] = {"php-cgi",const_cast<char *>((*reqHeader.find("File_Name:")).second.c_str()), NULL};
    char **env;
    env = setVaribels(reqHeader);
    execve(argv[0], argv, env);
    perror("execve");
}

int    exec_outfile(std::string inFile, std::map<std::string, std::string> reqHeader)
{
    std::string outFile("out_file");
    int in_fd = open(inFile.c_str(), O_WRONLY);
    int out_fd = open(outFile.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    pid_t pid = fork();
    if(pid == -1)
        return 1;
    if(pid == 0)
    {
        dup2(in_fd, 0);
        dup2(out_fd, 1);
        exec(reqHeader);
        exit(1);
    }
    waitpid(pid, NULL, WNOHANG);
    close(in_fd);
    close(out_fd);
    return 0;
}
int main()
{
    std::map<std::string, std::string> m;

    std::pair<std::string, std::string> p6("File_Name:","/Users/mait-aad/Desktop/webserver/cgi/index.php");
    m.insert(p6);
    std::pair<std::string, std::string> p("Content-Lengh:","");
    m.insert(p);
    std::pair<std::string, std::string> p1("Content-Type:","");
    m.insert(p1);
    std::pair<std::string, std::string> p4("Request_Method:","GET");
    m.insert(p4);
    std::pair<std::string, std::string> p5("Query-String:", "127.0.0.1:8080/s.php");
    m.insert(p5);
    std::pair<std::string, std::string> p0("Set-Cookie:", "");
    m.insert(p0);
    exec_outfile("inFile", m);
}