CC=g++
IN=robot.cpp
OUT=robot
FLAGS=-lglut -lGL -lGLU -lm -std=c++11


DEFAULT:
	rm $(OUT) 2> /dev/null ;\
	$(CC) $(IN) -o $(OUT) $(FLAGS) ;\
	./$(OUT)

.PHONY: DEFAULT

