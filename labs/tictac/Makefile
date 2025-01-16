all: game-check

move: Move.cpp
	g++ Move.cpp -c -std=c++17 -Wall -Wextra -Werror -Wno-unused-parameter

move-check: move move-check.cpp
	g++ move-check.cpp -c -std=c++17 -Wall -Wextra -Werror -Wno-unused-parameter 
	g++ Move.o move-check.o -o move-check -std=c++17
	del -f *.o

game-check: move board.cpp game-check.cpp
	g++ board.cpp -c -std=c++17 -Wall -Wextra -Werror -Wno-unused-parameter
	g++ game-check.cpp -c -std=c++17 -Wall -Wextra -Werror -Wno-unused-parameter
	g++ Move.o board.o game-check.o -o game-check -std=c++17
	del -f *.o

clean:
	del -f *.o *.exe