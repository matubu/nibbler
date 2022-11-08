# https://www.sfml-dev.org/tutorials/2.5/start-linux.php

LIBFLAG = -lsfml-graphics -lsfml-window -lsfml-system -L./lib
CPPFLAGS = -Wall -Wextra -std=c++17 -fPIC -O3 -I./include

SRCS = lib.cpp
OBJS = $(SRCS:.cpp=.o)

NAME = libsfml.so

$(NAME): $(OBJS)
	g++ ${CPPFLAGS} -shared ${OBJS} -o ${NAME} ${LIBFLAG}
