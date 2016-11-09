all: surface.o

run: all
	./surface.o

surface.o: surface.cpp
	g++ surface.cpp -o surface.o -lGL -lGLU -lglut -lm

clean:
	rm surface.o
