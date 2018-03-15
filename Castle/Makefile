CC=gcc
IN=castle.c
OUT=castle
FLAGS=-lglut -lGL -lGLU -lm


DEFAULT:
	rm $(OUT) 2> /dev/null ;\
	$(CC) $(IN) -o $(OUT) $(FLAGS) ;\
	./$(OUT)

.PHONY: DEFAULT

