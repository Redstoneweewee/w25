all: List Counter Iterator Index main

List: List.cpp List.h
	g++ -c List.cpp -std=c++17 -g -Wall -Wextra -Werror -Wno-unused-parameter

Counter: Counter.cpp Counter.h
	g++ -c Counter.cpp -std=c++17 -g -Wall -Wextra -Werror -Wno-unused-parameter

Iterator: Iterator.cpp
	g++ -c Iterator.cpp -std=c++17 -g -Wall -Wextra -Werror -Wno-unused-parameter



Index: Index.cpp Index.h
	g++ -c Index.cpp -std=c++17 -g -Wall -Wextra -Werror -Wno-unused-parameter


test: test.cpp
	g++ -c test.cpp -std=c++17 -g -Wall -Wextra -Werror -Wno-unused-parameter
	g++  List.o Index.o test.o -o test -std=c++17 -g -Wall -Wextra -Werror -Wno-unused-parameter
	del -f *o


main: main.cpp
	g++ -c main.cpp -std=c++17 -g -Wall -Wextra -Werror -Wno-unused-parameter
	g++ List.o Counter.o Iterator.o Index.o main.o -o main -std=c++17 -g -Wall -Wextra -Werror -Wno-unused-parameter
	del -f *o

clean:
	del -f *.o *.exe 