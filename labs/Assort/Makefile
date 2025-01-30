all: test

test: List.cpp test.cpp List.h
	g++ List.cpp -c -Wall -Wextra -Werror -Wno-unused-parameter
	g++ test.cpp -c -Wall -Wextra -Werror -Wno-unused-parameter
	g++ List.o test.o -o list -Wall -Wextra -Werror -Wno-unused-parameter
	del *.o

clean: 
	del *.exe *.o