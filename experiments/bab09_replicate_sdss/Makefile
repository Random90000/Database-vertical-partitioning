CC=g++
#CXXFLAGS=--std=c++11 -g -L/cstore/intelstudio/tbb/lib/intel64/gcc4.4 -ltbb_debug -O0
#export LD_LIBRARY_PATH=/cstore/intelstudio/tbb/lib/intel64/gcc4.4:$LD_LIBRARY_PATH
CXXFLAGS=--std=c++14 -g -ltbb -O2
PQLIBS=-lpqxx -lpq

SRC_MATRIX_TEST = matrix_test.cpp matrix.cpp bab.cpp measures.cpp bab11.cpp
OBJ_MATRIX_TEST = matrix_test.o matrix.o bab.o measures.o bab11.cpp

SRC_MATRIX_TEST_PARALLEL = matrix_test_parallel.cpp pbab.hpp matrix.cpp bab.cpp measures.cpp
OBJ_MATRIX_TEST_PARALLEL = matrix_test_parallel.o pbab.o matrix.o bab.o measures.o

SRC_VPART = main.cpp DB.cpp queries.cpp matrix.cpp util.cpp bab.cpp measures.cpp bab11.cpp
OBJ_VPART = main.o DB.o queries.o matrix.o util.o bab.o measures.o bab11.o

vpart: $(OBJ_VPART)
	$(CC) $(OBJ_VPART) $(CXXFLAGS) $(PQLIBS) -o $@

matrix_test: $(OBJ_MATRIX_TEST)
	$(CC) $(OBJ_MATRIX_TEST) $(CXXFLAGS) -o $@

matrix_test_parallel: $(OBJ_MATRIX_TEST_PARALLEL)
	$(CC) $(OBJ_MATRIX_TEST_PARALLEL) $(CXXFLAGS) -o $@

%.o: %.cc
	$(CC) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -f matrix_test
	@rm -f matrix_test_parallel
	@rm -f vpart
	@rm -f *.o

.PHONY: clean
