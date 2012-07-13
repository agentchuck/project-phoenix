CC=g++
SOURCE_FILES=main.cpp

go: main.cpp world.cpp
	$(CC) main.cpp world.cpp -I. -o go

.PHONY: clean
clean:
	rm -f *.o
