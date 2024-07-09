DEBUG?=0
D?=2
R?=32
CXX=mpic++
FLAGS=-std=c++20 -DPTDIM=$(D) -DFPSIZE=$(R)
INCS=-I./include

ifeq ($(DEBUG),1)
FLAGS+=-DDEBUG -O0 -g -fsanitize=address -fno-omit-frame-pointer
else
FLAGS+=-O2
endif

all: test_itree test_vcell

test_itree: tests/test_itree.cpp include
	$(CXX) -o $@ $(FLAGS) $(INCS) $<

test_vcell: tests/test_vcell.cpp include
	$(CXX) -o $@ $(FLAGS) $(INCS) $<

clean:
	rm -rf *.dSYM test_itree test_vcell
