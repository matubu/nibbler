# https://www.sfml-dev.org/tutorials/2.5/start-linux.php

LIBFLAG = -lsfml-graphics -lsfml-window -lsfml-system -L./lib
CPPFLAGS = -Wall -Wextra -std=c++17 -fPIC -O3 -I./include

all:
	g++ ${CPPFLAGS} -shared lib.cpp -o libsfml.so ${LIBFLAG}