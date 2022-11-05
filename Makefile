Name=nibbler

all:
	make -C ./raylib
	make -C ./sdl2
	make -C ./sfml
	g++ main.cpp -o $(Name)
	./nibbler

fclean:
	rm -f ./raylib/libraylib.so
	rm -f ./sdl2/libsdl2.so
	rm -f ./sfml/libsfml.so
	rm -f $(Name)

re: fclean all