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

all: create_points point_info graph_diff build_rgraph
test: test_itree test_vcell test_graphutils test_ptidx test_pts

create_points: src/create_points.cpp include
	$(CXX) -o $@ $(FLAGS) $(INCS) $<

graph_diff: src/graph_diff.cpp include
	$(CXX) -o $@ $(FLAGS) $(INCS) $<

point_info: src/point_info.cpp include
	$(CXX) -o $@ $(FLAGS) $(INCS) $<

build_rgraph: src/build_rgraph.cpp include
	$(CXX) -o $@ $(FLAGS) $(INCS) $<

test_itree: tests/test_itree.cpp include
	$(CXX) -o $@ $(FLAGS) $(INCS) $<

test_vcell: tests/test_vcell.cpp include
	$(CXX) -o $@ $(FLAGS) $(INCS) $<

test_ptidx: tests/test_ptidx.cpp include
	$(CXX) -o $@ $(FLAGS) $(INCS) $<

test_pts: tests/test_pts.cpp include
	$(CXX) -o $@ $(FLAGS) $(INCS) $<

test_graphutils: tests/test_graphutils.cpp include
	$(CXX) -o $@ $(FLAGS) $(INCS) $<

clean:
	rm -rf *.dSYM test_itree test_vcell test_graphutils test_ptidx test_pts create_points point_info graph_diff build_rgraph
