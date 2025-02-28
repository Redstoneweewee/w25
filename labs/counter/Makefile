all: List test

List: List.cpp List.h
	g++ -c List.cpp -std=c++17 -g -Wall -Wextra -Werror -Wno-unused-parameter

test: test.cpp
	g++ -c test.cpp -std=c++17 -g -Wall -Wextra -Werror -Wno-unused-parameter
	g++ List.o test.o -o test -std=c++17 -g -Wall -Wextra -Werror -Wno-unused-parameter
	del -f *o

clean:
	del -f *.o *.exe 