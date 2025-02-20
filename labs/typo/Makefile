all: heap point wordList main

heap:
	g++ -c Heap.cpp -std=c++17 -g -Wall -Wextra -Werror -Wno-unused-parameter

point:
	g++ -c Point.cpp -std=c++17 -g -Wall -Wextra -Werror -Wno-unused-parameter

wordList:
	g++ -c WordList.cpp -std=c++17 -g -Wall -Wextra -Werror -Wno-unused-parameter

main:
	g++ -c Main.cpp -std=c++17 -g -Wall -Wextra -Werror -Wno-unused-parameter
	g++ Heap.o Point.o WordList.o Main.o -o typo -std=c++17 -g -Wall -Wextra -Werror -Wno-unused-parameter
	del -f *.o

test:
	g++ -c test.cpp -std=c++17 -g -Wall -Wextra -Werror -Wno-unused-parameter
	g++ Heap.o Point.o WordList.o test.o -o test -std=c++17 -g -Wall -Wextra -Werror -Wno-unused-parameter
	del -f *.o


clean:
	del -f *.o *.exe 