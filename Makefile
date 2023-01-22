CFLAGS = -g -Wall -Wextra -pedantic -std=c++14

all: test

test: test.o
	g++ $(CFLAGS) test.o -o test
test.o: test.cpp Deque.hpp
	g++ $(CFLAGS) -c test.cpp

clean:
	rm *.o test
run: all
	./test
memcheck: all
	valgrind ./test
memcheck++: all
	valgrind --leak-check=full ./test
