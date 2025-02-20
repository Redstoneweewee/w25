all: heap point wordList main

heap: Heap.cpp Heap.h
	g++ -o Heap.cpp -std=c++17 -g -Wall -Wextra -Werror -Wno-unused-parameter

point: Point.cpp
	g++ -o Point.cpp -std=c++17 -g -Wall -Wextra -Werror -Wno-unused-parameter

wordList: WordList.cpp
	g++ -o WordList.cpp -std=c++17 -g -Wall -Wextra -Werror -Wno-unused-parameter

main: Main.cpp
	g++ -o Main.cpp -std=c++17 -g -Wall -Wextra -Werror -Wno-unused-parameter
	g++ Heap.o Point.o WordList.o Main.o typo -std=c++17 -g -Wall -Wextra -Werror -Wno-unused-parameter
	del -f *.o

clean:
	del -f *.o *.exe 