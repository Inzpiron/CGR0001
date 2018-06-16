FILE = src/main.cpp
OLDFILE = old_src/main.cpp
OUT = ray
FLAGS = -lm -fopenmp -lsfml-system -lsfml-graphics -lsfml-window -lpthread -std=c++11
CC = g++

all: new

new: $(FILE)
	$(CC) $(FILE) -o $(OUT) $(FLAGS)

old: $(OLDFILE)
	$(CC) $(OLDFILE) -o $(OUT) $(FLAGS)

.PHONY: ALL
