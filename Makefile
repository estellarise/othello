othello.out: board.o othello.o
	g++ -o othello.out board.o othello.o
othello.o: othello.cpp
	g++ -c othello.cpp

board.o: board.cpp board.h
	g++ -c board.cpp

debug:
	g++ -g -o debugOthello.out othello.cpp board.cpp 

clean:
	rm -f *.out *.o *.stackdump *~

backup:
	test -d backups || mkdir backups
	cp *.cpp backups
	cp *.h backups
	cp Makefile backups