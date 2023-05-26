SRC = server.cpp http_TcpServer.cpp methods/http_response.cpp cgi/cgi.cpp parsing/request.cpp parsing/pars.cpp  methods/delete.cpp parsing/func.cpp parsing/Location.cpp parsing/server.cpp

NAME = webserv

FLAG = c++ -std=c++98 -Wall -Wextra -Werror -fsanitize=address -g3
all : ${NAME}

${NAME} :
	@${FLAG} ${SRC} -o ${NAME}

clean : 
	@ rm -f ${NAME}
fclean : clean

re : fclean all
