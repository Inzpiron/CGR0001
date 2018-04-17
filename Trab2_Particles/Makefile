FLAGS=-lsfml-system -lsfml-window -lsfml-graphics -pthread -lglut -lGL -lGLU -lm -std=c++11

all: fireworks

fireworks: fireworks.cpp fireworks.hpp
	g++ -o fireworks fireworks.cpp $(FLAGS)

.PHONY: all fireworks
