run: all
	./surface.o

all: surface.o

surface.o: surface.h surface.cpp
	g++ surface.cpp -o surface.o -lGL -lGLU -lglut -lm

clean:
	rm surface.o
