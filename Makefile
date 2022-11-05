Flags = -I.

all:
	g++ $(Flags) main.cpp -o main
	g++ $(Flags) sfml/lib.cpp -o libsfml.so -shared -fPIC -lsfml-graphics -lsfml-window -lsfml-system
	./main