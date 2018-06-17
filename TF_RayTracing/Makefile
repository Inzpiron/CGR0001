NEWFILE = new_src/main.cpp
OURFILE = our_src/main.cpp
OUT = ray
FLAGS = -lm -fopenmp -lsfml-system -lsfml-graphics -lsfml-window -lpthread -std=c++11
CC = g++

all: our

new: $(NEWFILE)
	$(CC) $(NEWFILE) -o $(OUT) $(FLAGS)

our: $(OLDFILE)
	$(CC) $(OURFILE) -o $(OUT) $(FLAGS)

.PHONY: ALL
