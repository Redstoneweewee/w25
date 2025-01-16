all: combine

move: Move.cpp
	g++ Move.cpp -c -std=c++17 -Wall -Wextra -Werror -Wno-unused-parameter

move-check: move-check.cpp
	g++ move-check.cpp -c -std=c++17 -Wall -Wextra -Werror -Wno-unused-parameter 

combine: move move-check
	g++ Move.o move-check.o -o move -std=c++17
	del -f *.o

clean:
	del -f *.o move.exe