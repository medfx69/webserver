#include "cgi.hpp"

char  **setVaribels(std::map<std::string, std::string> reqHeader){
    if (reqHeader.find("Content-Type:") == reqHeader.end())
    {
        std::pair<std::string, std::string> p ("Content-Type:", "");
        reqHeader.insert(p);
    }
    if (reqHeader.find("Content-Length:") == reqHeader.end())
    {
        std::pair<std::string, std::string> p ("Content-Length:", "0");
        reqHeader.insert(p);
    }
    if (reqHeader.find("Cookie:") == reqHeader.end())
    {
        std::pair<std::string, std::string> p ("Cookie:", "");
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
    char **s;
    s = new char*[5];
    std::string s0 = "CONTENT_TYPE: " + (*reqHeader.find("Content-Type:")).second;
    s[0] = new char[s0.size()];
    std::strcpy(s[0], const_cast<char *>(s0.c_str()));
    std::string s1 = "REQUEST_METHOD: " + (*reqHeader.find("Request-Method:")).second;
    s[1] = new char[s1.size()];
    std::strcpy(s[1], const_cast<char *>(s1.c_str()));
    std::string s2 = "CONTENT_LENGTH: " + (*reqHeader.find("Content-Length:")).second;
    s[2] = new char[s2.size()];
    std::strcpy(s[2], const_cast<char *>(s2.c_str()));
    std::string s3 = "QUERY_STRING: " + (*reqHeader.find("Query-String:")).second;
    s[3] = new char[s3.size()];
    std::strcpy(s[3], const_cast<char *>(s3.c_str()));
    std::string s4 = "HTTP_COOKIE: " + (*reqHeader.find("Cookie:")).second;
    s[4] = new char[s4.size()];
    std::strcpy(s[4], const_cast<char *>(s4.c_str()));

    s[5] = NULL;
    return s;
}

void    exec(std::map<std::string, std::string> reqHeader){
    char* argv[] = {const_cast<char *>((*reqHeader.find("Program_Name:")).second.c_str()),const_cast<char *>((*reqHeader.find("File_Name:")).second.c_str()), NULL};
    char **env;
    env = setVaribels(reqHeader);
    execve(argv[0], argv, env);
    exit(1);
}

std::string    response::exec_outfile(std::string inFile, std::map<std::string, std::string> reqHeader){
    std::string outFile("out_file");
    std::string outFileStr;
    int         in_fd = open(inFile.c_str(), O_WRONLY);
    int         out_fd = open(outFile.c_str(), O_WRONLY | O_CREAT);

    pid_t pid = fork();
    if (pid == -1)
        return "";
    if (pid == 0){

        dup2(in_fd, 0);
        dup2(out_fd, 1);
        exec(reqHeader);
    }
    waitpid(pid, NULL, WNOHANG);
    close(in_fd);
    close(out_fd);

    std::ifstream file(outFile);
    std::stringstream buffer;
    buffer << file.rdbuf();
    outFileStr = buffer.str();
    if (outFileStr.substr(0, 7) == "Status:")
        return generateResponse(stoi(outFileStr.substr(8, 3)));
    std::string cnL("HTTP/1.1 200 Ok\r\nContent-Length: ");
    std::stringstream ss;
    ss << (outFileStr.substr(outFileStr.find("\r\n\r\n") + 4, outFileStr.size())).size();
    cnL += ss.str();
    cnL += "\n";
    cnL += outFileStr;
    std::cout << cnL << std::endl;
    file.close();
    return cnL;
}
// int main()
// {
//     std::map<std::string, std::string> m;

//     std::pair<std::string, std::string> p6("File_Name:","/Users/mait-aad/Desktop/webserver/cgi/index.php");
//     m.insert(p6);
//     std::pair<std::string, std::string> p8("Program_Name:","php-cgi");
//     m.insert(p8);
//     std::pair<std::string, std::string> p("Content-Length:","");
//     m.insert(p);
//     std::pair<std::string, std::string> p1("Content-Type:","");
//     m.insert(p1);
//     std::pair<std::string, std::string> p4("Request-Method:","GET");
//     m.insert(p4);
//     std::pair<std::string, std::string> p5("Query-String:", "127.0.0.1:8080/s.php");
//     m.insert(p5);
//     std::pair<std::string, std::string> p0("Set-Cookie:", "");
//     m.insert(p0);
//     exec_outfile("inFile", m);
// }