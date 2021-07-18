DEPENDENCIES = board.hpp piece.hpp

myChess: main.o piece.o board.o
	g++ main.o piece.o board.o -o myChess

main.o: main.cpp $(DEPENDENCIES)
	g++ -c main.cpp

piece.o: piece.cpp $(DEPENDENCIES)
	g++ -c piece.cpp

board.o: board.cpp $(DEPENDENCIES)
	g++ -c board.cpp
