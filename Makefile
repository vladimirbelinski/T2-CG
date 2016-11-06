all: surface

surface: surface.cpp
	g++ surface.cpp -o surface.o -lGL -lGLU -lglut -lm

clean:
	rm surface.o
