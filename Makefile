Name=nibbler

LIBFLAG = -lsfml-audio -L./sfml/lib
CPPFLAGS = -Wall -Wextra -Werror -std=c++17 -O3 -I./sfml/include

all:
	make -C ./raylib
	make -C ./sdl2
	make -C ./sfml
	g++ $(CPPFLAGS) main.cpp Audio.cpp -o $(Name) $(LIBFLAG)

fclean:
	rm -f ./raylib/libraylib.so
	rm -f ./sdl2/libsdl2.so
	rm -f ./sfml/libsfml.so
	rm -f $(Name)

re: fclean all