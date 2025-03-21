all: point map main

point: Point.cpp Point.h
	g++ -c Point.cpp -std=c++17 -g -Wall -Wextra -Werror -Wno-unused-parameter

map: Map.cpp Map.h
	g++ -c Map.cpp -std=c++17 -g -Wall -Wextra -Werror -Wno-unused-parameter



main: main.cpp
	g++ -c main.cpp -std=c++17 -g -Wall -Wextra -Werror -Wno-unused-parameter
	g++ main.o Point.o Map.o -o main -g -Wall -Wextra -Werror -Wno-unused-parameter
	del -f *.o

test: point map test.cpp
	g++ -c test.cpp -std=c++17 -g -Wall -Wextra -Werror -Wno-unused-parameter
	g++ test.o Point.o Map.o -o test -g -Wall -Wextra -Werror -Wno-unused-parameter
	del -f *.o

clean:
	del -f *.o *.exe