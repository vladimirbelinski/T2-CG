all: surface

surface: surface.cpp
	g++ surface.cpp -lGL -lGLU -lglut -o surface

clean:
	rm surface
