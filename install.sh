# Install SFML
brew install sfml

# Install SDL2
brew install sdl2

# In case we want to write stuff in the game this is the lib for sdl2
# brew install sdl2_ttf 

if [ ! -d ./raylib/raylib/src ]; then
	git clone https://github.com/raysan5/raylib.git ./raylib/raylib
	make -C ./raylib/raylib/src PLATFORM=PLATFORM_DESKTOP
fi

# Need to modify for 42 Mac
path=/opt/homebrew/Cellar

# SFML links
ln -sf $path/sfml/*/include ./sfml/include
ln -sf $path/sfml/*/lib ./sfml/lib

# SDL2 links
ln -sf $path/sdl2/*/include ./sdl2/include
ln -sf $path/sdl2/*/lib ./sdl2/lib