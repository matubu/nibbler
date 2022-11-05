# https://github-wiki-see.page/m/raysan5/raylib/wiki/Working-on-macOS
# -Llib/ -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL 
LIBFLAG =  -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL -L./raylib/src
CPPFLAGS = -fPIC -O3 -std=c++11 -I./raylib/src

all:
	g++ ${CPPFLAGS} -shared lib.cpp ./raylib/src/libraylib.a -o libraylib.so ${LIBFLAG}