CC = g++
strict_options = -Wall -pedantic

sudoku: initialise.o preprocess.o printing.o readpuzzle.o stochastic.o compare.o main.o
	$(CC) -g -pg -o $@ $^

main.o: main.cpp model.h
	$(CC) -g -pg -o $@ -c $<

initialise.o: initialise.cpp model.h
	$(CC) -g -pg -o $@ -c $<

preprocess.o: preprocess.cpp
	$(CC) -g -pg -o $@ -c $<

printing.o: printing.cpp
	$(CC) -g -pg -o $@ -c $^

readpuzzle.o: readpuzzle.cpp model.h
	$(CC) -g -pg -o $@ -c $<

stochastic.o: stochastic.cpp printing.o model.h preprocess.o
	$(CC) -g -pg -o $@ -c $<

initialfill.o: initialfill.cpp
	$(CC) -g -pg -o $@ -c $<

compare.o: compare.cpp
	$(CC) -g -pg -o $@ -c $<

clean :
	rm -f *.o *~ sudoku *.gch
	rm -f result


#initial fill with min conflicting value
#Cut out singleton domained cell
#