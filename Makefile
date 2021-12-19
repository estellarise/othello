othello.out: board.o othello.o
	g++ -o othello.out board.o othello.o
othello.o: othello.cpp othello.hpp board.cpp
	g++ -c othello.cpp

board.o: board.cpp board.hpp
	g++ -c board.cpp

debug:
	g++ -g -o debugOthello.out othello.cpp board.cpp 

clean:
	rm -f *.out *.o *.stackdump *~

backup:
	test -d backups || mkdir backups
	cp *.cpp backups
	cp *.hpp backups
	cp Makefile backups