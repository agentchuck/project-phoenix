CC=g++
SOURCE_FILES=main.cpp \
	     world.cpp
TEAM_CODE=96495776

all: go lifter

go: main.cpp world.cpp world.hh
	$(CC) main.cpp world.cpp -I. -o go

lifter: lifter.cpp world.cpp world.hh includes.hh
	$(CC) lifter.cpp world.cpp -I. -o lifter
	cp lifter ..

.PHONY: clean
clean:
	rm -f *.o

submit: lifter

