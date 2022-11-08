NAME=nibbler

LIBFLAG = -lsfml-audio -L./sfml/lib
CPPFLAGS = -Wall -Wextra -Werror -std=c++17 -O3 -I./sfml/include

SRCS = main.cpp Audio.cpp
OBJS = $(SRCS:.cpp=.o)

${NAME}: ${OBJS}
	make libs
	g++ ${CPPFLAGS} ${OBJS} -o ${NAME} ${LIBFLAG}

libs:
	if [ "$(shell uname -s)" = "Darwin" ]; then \
		make -C ./raylib; \
	fi	
	make -C ./sdl2
	make -C ./sfml

clean:
	rm -f ${OBJS}

fclean: clean
	rm -f ./raylib/*.so
	rm -f ./sdl2/*.so
	rm -f ./sfml/*.so
	rm -f ./raylib/*.o
	rm -f ./sdl2/*.o
	rm -f ./sfml/*.o
	rm -f $(NAME)

re: fclean ${NAME}