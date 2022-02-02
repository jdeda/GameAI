# HW1 Makefile.

UBUNTU_LIB=/usr/lib/x86_64-linux-gnu

# Builds all.
all: main

# Dependencies of main.o
main: main.o
	g++ main.o -o main -lsfml-graphics -lsfml-window -lsfml-system -L$(UBUNTU_LIB)

# Dependencies of main.cp.
main.o: main.cpp
	g++ -c main.cpp

# Clean.
clean:
	rm -rf main.o
	rm -rf main
