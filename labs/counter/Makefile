all: List Counter test

List: List.cpp List.h
	g++ -c List.cpp -std=c++17 -g -Wall -Wextra -Werror -Wno-unused-parameter

Counter: Counter.cpp Counter.h
	g++ -c Counter.cpp -std=c++17 -g -Wall -Wextra -Werror -Wno-unused-parameter

test: test.cpp
	g++ -c test.cpp -std=c++17 -g -Wall -Wextra -Werror -Wno-unused-parameter
	g++ List.o Counter.o test.o -o test -std=c++17 -g -Wall -Wextra -Werror -Wno-unused-parameter
	del -f *o

clean:
	del -f *.o *.exe 