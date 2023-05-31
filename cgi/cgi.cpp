#include "../methods/http_response.hpp"

char **setVaribels(std::map<std::string, std::string> reqHeader)
{
	if (reqHeader.find("Content-Type:") == reqHeader.end())
	{
		std::pair<std::string, std::string> p("Content-Type:", "");
		reqHeader.insert(p);
	}
	if (reqHeader.find("Content-Length:") == reqHeader.end())
	{
		std::pair<std::string, std::string> p("Content-Length:", "0");
		reqHeader.insert(p);
	}
	if (reqHeader.find("Cookie:") == reqHeader.end())
	{
		std::pair<std::string, std::string> p("Cookie:", "");
		reqHeader.insert(p);
	}
	if (reqHeader.find("Request-Method:") == reqHeader.end())
	{
		std::pair<std::string, std::string> p("Request-Method:", "");
		reqHeader.insert(p);
	}
	if (reqHeader.find("Redirect-Status:") == reqHeader.end())
	{
		std::pair<std::string, std::string> p("Redirect-Status:", "CGI");
		reqHeader.insert(p);
	}

	char **s;
	s = new char *[9];
	std::string s0 = "CONTENT_TYPE=" + (*reqHeader.find("Content-Type:")).second;
	s[0] = strdup(const_cast<char *>(s0.c_str()));
	std::string s1 = "REQUEST_METHOD=" + (*reqHeader.find("Request-Method:")).second;
	s[1] = strdup(const_cast<char *>(s1.c_str()));
	std::string s2 = "CONTENT_LENGTH=" + (*reqHeader.find("Content-Length:")).second;
	s[2] = strdup(const_cast<char *>(s2.c_str()));
	std::string s3;
	if ((*reqHeader.find("Query-String:")).second.size())
		s3 = "QUERY_STRING=" + (*reqHeader.find("Query-String:")).second.substr(1, (*reqHeader.find("Query-String:")).second.size());
	s[3] = strdup(const_cast<char *>(s3.c_str()));
	std::string s4 = "HTTP_COOKIE=" + (*reqHeader.find("Cookie:")).second;
	s[4] = strdup(const_cast<char *>(s4.c_str()));
	std::string s5 = "REDIRECT_STATUS=" + (*reqHeader.find("Redirect-Status:")).second;
	s[5] = strdup(const_cast<char *>(s5.c_str()));
	std::string s6 = "SCRIPT_NAME=" + (*reqHeader.find("File_Name:")).second;
	s[6] = strdup(const_cast<char *>(s6.c_str()));
	std::string s7 = "SCRIPT_FILENAME=" + (*reqHeader.find("File_Name:")).second;
	s[7] = strdup(const_cast<char *>(s7.c_str()));
	s[8] = NULL;
	return s;
}

void    exec(std::map<std::string, std::string> reqHeader){
	char* argv[] = {const_cast<char *>((*reqHeader.find("Program_Name:")).second.c_str()),const_cast<char *>((*reqHeader.find("File_Name:")).second.c_str()), NULL};
	char **env;
	env = setVaribels(reqHeader);
	execve(argv[0], argv, env);
}

std::string response::exec_outfile(std::string inFile, std::map<std::string, std::string> reqHeader)
{
	std::string outFile("/tmp/out_file");
	std::string outFileStr;
	int in_fd = open(inFile.c_str(), O_RDONLY);
	int out_fd = open(outFile.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0755);

	pid_t pid = fork();
	if (pid == -1)
		return "";
	if (pid == 0)
	{

		dup2(in_fd, 0);
		dup2(out_fd, 1);
		exec(reqHeader);
		exit(1);
	}
	int start_time = time(NULL);
	int timeout = 120;
	while (time(NULL) - start_time < timeout)
	{
		pid_t rc = waitpid(pid, NULL, WNOHANG);
		if (rc != 0)
		{
			if (rc > 0)
			{
				close(in_fd);
				close(out_fd);
				std::ifstream file(outFile);
				std::stringstream buffer;
				buffer << file.rdbuf();
				outFileStr = buffer.str();
				if (outFileStr.size() == 0)
					return generateResponse(400);
				if (outFileStr.substr(0, 7) == "Status:")
				{
					return generateResponse(stoi(outFileStr.substr(outFile.find("Status:") + 9, 3)));
				}
				std::string cnL("HTTP/1.1 200 Ok\r\nContent-Length: ");
				std::stringstream ss;
				ss << (outFileStr.substr(outFileStr.find("\r\n\r\n") + 4, outFileStr.size())).size();
				cnL += ss.str();
				cnL += "\n";
				cnL += outFileStr;
				file.close();
				return cnL;
			}
		}
	}
	kill(pid, SIGTERM);
	return generateResponse(404);
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
