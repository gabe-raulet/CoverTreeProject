DEBUG?=0
CXX=clang++
FLAGS=-std=c++20

ifeq ($(DEBUG),1)
FLAGS+=-DDEBUG -O0 -g -fsanitize=address -fno-omit-frame-pointer
else
FLAGS+=-O2
endif

FILES=graphutils.h graphutils.hpp fileinfo.h

all: main test_shuffle

main: main.cpp $(FILES)
	$(CXX) -o $@ $(FLAGS) $<

test_shuffle: test_shuffle.cpp $(FILES)
	$(CXX) -o $@ $(FLAGS) $<

clean:
	rm -rf *.{dSYM,txt} main test_shuffle
