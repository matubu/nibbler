Name=nibbler

LIBFLAG = -lsfml-audio -L./sfml/lib
CPPFLAGS = -O3 -I./sfml/include

all:
	make -C ./raylib
	make -C ./sdl2
	make -C ./sfml
	g++ $(CPPFLAGS) main.cpp -o $(Name) $(LIBFLAG)
	ALSOFT_LOGLEVEL=0 ./nibbler

fclean:
	rm -f ./raylib/libraylib.so
	rm -f ./sdl2/libsdl2.so
	rm -f ./sfml/libsfml.so
	rm -f $(Name)

re: fclean all