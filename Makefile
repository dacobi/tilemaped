all:
	g++ -std=c++17 -ggdb tilemaped.cpp -I/usr/include/SDL2/ -lSDL2 -lSDL2_image -lSDL2_ttf -o tilemaped
