# https://github-wiki-see.page/m/raysan5/raylib/wiki/Working-on-macOS
# -Llib/ -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL 
LIBFLAG =  -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL -L./raylib/src
CPPFLAGS = -Wall -Wextra -std=c++17 -fPIC -O3 -I./raylib/src

NAME=libraylib.so

SRCS = lib.cpp
OBJS = $(SRCS:.cpp=.o)

${NAME}: ${OBJS}
	if [ "$(shell uname -s)" = "Darwin" ]; then \
		g++ ${CPPFLAGS} -shared ${OBJS} ./raylib/src/libraylib.a -o ${NAME} ${LIBFLAG}; \
	fi	
