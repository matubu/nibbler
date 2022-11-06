# https://github-wiki-see.page/m/raysan5/raylib/wiki/Working-on-macOS
# -Llib/ -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL 
LIBFLAG =  -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL -L./raylib/src
CPPFLAGS = -fPIC -O3 -I./raylib/src

all:
	if [ "$(shell uname -s)" = "Darwin" ]; then \
		g++ ${CPPFLAGS} -shared lib.cpp ./raylib/src/libraylib.a -o libraylib.so ${LIBFLAG}; \
	fi
