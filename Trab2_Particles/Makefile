FLAGS=-lsfml-system -lsfml-window -lsfml-graphics -pthread

all: fireworks

fireworks: fireworks.cpp fireworks.hpp
	g++ -o fireworks fireworks.cpp fireworks.hpp

.PHONY: clean all

clean:
	-rm *.o
	-rm client
	-rm server
	-rm hw.h
	-rm hw_clnt.c
	-rm hw_svc.c