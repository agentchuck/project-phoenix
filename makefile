CC=g++
SOURCE_FILES=main.cpp

main: main.cpp
	$(CC) main.cpp -o main

.PHONY: clean
clean:
	rm -f *.o
