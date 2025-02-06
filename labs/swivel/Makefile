all: test

test: Set.cpp Node.cpp test.cpp
	g++ Set.cpp -c
	g++ Node.cpp -c
	g++ test.cpp -c
	g++ Set.o Node.o test.o -o test -std=c++17 -Wall -Wextra -Werror -Wno-unused-parameter
	del -f *.o

clean:
	del -f *.o *.exe