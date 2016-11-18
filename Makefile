all: bmp surface.o

run: all
	./surface.o

bmp: bmp.cpp
	g++ -c -g bmp.cpp -o bmp.o

surface.o: surface.h surface.cpp
	g++ bmp.o -Wall -O2 -lm -g surface.cpp -lGL -lGLU -lglut -o surface.o

clean:
	rm bmp.o surface.o
