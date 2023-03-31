SRC = server.cpp http_TcpServer.cpp

NAME = webserv

FLAG = c++  #-fsanitize=address -g3#-Wall -Wextra -Werror -std=c++98

all : ${NAME}

${NAME} :
	@${FLAG} ${SRC} -o ${NAME}

clean : 
	@ rm -f ${NAME}
fclean : clean

re : fclean all