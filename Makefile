SRC =	server.cpp \
		cgi/cgi.cpp \
		http_TcpServer.cpp	\
		methods/http_response.cpp	\
		parsing/request.cpp	\
		parsing/pars.cpp	\
		methods/delete.cpp	\
		methods/get.cpp	\
		methods/post.cpp	\
		parsing/func.cpp	\
		parsing/Location.cpp	\
		parsing/server.cpp	\

NAME = webserv

FLAG = c++ -std=c++98 -Wall -Wextra
all : ${NAME}

${NAME} :
	@${FLAG} ${SRC} -o ${NAME}

clean : 
	@ rm -f ${NAME}
fclean : clean

re : fclean all
