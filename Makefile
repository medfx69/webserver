SRC = server.cpp http_TcpServer.cpp methods/http_response.cpp parsing/request.cpp parsing/pars.cpp  parsing/func.cpp parsing/Location.cpp parsing/server.cpp

NAME = webserv

FLAG = c++ -g  -Wall -Wextra -Werror -std=c++98

all : ${NAME}

${NAME} :
	@${FLAG} ${SRC} -o ${NAME}

clean : 
	@ rm -f ${NAME}
fclean : clean

re : fclean all