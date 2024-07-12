DEBUG?=0
CXX=clang++
FLAGS=-std=c++20

ifeq ($(DEBUG),1)
FLAGS+=-DDEBUG -O0 -g -fsanitize=address -fno-omit-frame-pointer
else
FLAGS+=-O2
endif

all: main

main: main.cpp graphutils.h graphutils.hpp fileinfo.h
	$(CXX) -o $@ $(FLAGS) $<

clean:
	rm -rf *.dSYM main
