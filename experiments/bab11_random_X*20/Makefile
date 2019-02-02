CC=g++
CXXFLAGS=--std=c++14 -g -ltbb -O2

SRC_MATRIX_TEST = main.cpp matrix.cpp bab.cpp measures.cpp bab11.cpp
OBJ_MATRIX_TEST = main.o matrix.o bab.o measures.o bab11.cpp

matrix_test: $(OBJ_MATRIX_TEST)
	$(CC) $(OBJ_MATRIX_TEST) $(CXXFLAGS) -o $@

%.o: %.cc
	$(CC) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -f matrix_test
	@rm -f *.o

.PHONY: clean
