# https://medium.com/@edkins.sarah/set-up-sdl2-on-your-mac-without-xcode-6b0c33b723f7

LIBFLAG = -L./lib
CPPFLAGS = -fPIC -O3 -std=c++11 -I./include

all:
	g++ ${CPPFLAGS} -shared lib.cpp -o libsdl2.so ${LIBFLAG}