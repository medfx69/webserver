SRC = server.cpp #https_Tcpserver.cpp

NAME = webserv

FLAG = c++ -Wall -Wextra -Werror -std=c++98 -fsanitize=address

all : ${NAME}

${NAME} :
	@${FLAG} ${SRC} -o ${NAME}

clean : 
	@ rm -f ${NAME}
fclean : clean

re : fclean all