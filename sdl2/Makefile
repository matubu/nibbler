# https://medium.com/@edkins.sarah/set-up-sdl2-on-your-mac-without-xcode-6b0c33b723f7

LIBFLAG = -lSDL2 -L./lib
CPPFLAGS = -Wall -Wextra -std=c++17 -fPIC -O3 -I./include

SRCS = lib.cpp
OBJS = $(SRCS:.cpp=.o)

NAME = libsdl2.so

$(NAME): $(OBJS)
	g++ ${CPPFLAGS} -shared ${OBJS} -o ${NAME} ${LIBFLAG}
