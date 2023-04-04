CC = gcc
CFLAGS = -Wall -Wextra -std=c11 
LIBS = -lcheck
OS := $(shell uname -s)


ifeq ($(OS),Linux)
	LINUX_LIBS=-lsubunit -lrt -lpthread -lm
endif

.PHONY: install uninstall build tests dist dvi clean test
all: install tests

build:
	gcc -c $(CFLAGS) parser.c parser.h
	gcc -c $(CFLAGS) transformations.c transformations.h

tests: build
	gcc $(CFLAGS) test.c  *.o -o out $(LIBS) $(LINUX_LIBS)
	./out

# C8_3DViewer_v1.0-1/src/build_3d/3d.app/Contents/MacOS
install:
	rm -rf build_3d
	mkdir build_3d
	cd build_3d/ && qmake ../3d && make && pwd
ifeq ($(OS),Darwin)
	ls ./build_3d/3d.app/Contents/MacOS/3d
	cp ./build_3d/3d.app/Contents/MacOS/3d ./build_3d/3d 
	./build_3d/3d
endif	

dvi:
	open READMI.md

uninstall: clean
	rm -rf build_3d

dist: install
	rm -rf dist_3d
	mkdir dist_3d
	tar -cf dist_3d/3d.tar build_3d

gcov_report: test.c build
	clear
	gcc $(CFLAGS) -g test.c *.o -o coverage.o $(LIBS) $(LINUX_LIBS) -fprofile-arcs -ftest-coverage  
	./coverage.o
	lcov -c -d . -o coverage.info
	genhtml coverage.info -o coverage
	open coverage/index.html

clean:
	- rm *.o *.h.gch
	- rm -rf *.gcda *.gcno *.info
	- rm -rf coverage 
	- rm -rf out
	- rm -rf dist_3d

lint:
	cp ../materials/linters/.clang-format .
	clang-format -i *.c *.h
	clang-format -n *.c *.h
	rm .clang-format

leaks:
	valgrind --leak-check=full  --show-leak-kinds=all --track-origins=yes ./out