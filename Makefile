CC = g++
CFLAGS = --std=c++20 -Wall -pedantic -g
LIB = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -lboost_unit_test_framework

DEPS = Pong.hpp

OBJECTS = Pong.o

.PHONY: all clean Pongers

all: Pongers

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c $<

Pongers: main.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

clean:
	rm -f *.o Pongers

lint:
	cpplint *.cpp *.hpp