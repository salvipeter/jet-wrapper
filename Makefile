all: jet-test

INCLUDES=-I../libgeom -I/usr/include/eigen3
LIBRARIES=-L../libgeom/release -lgeom -lCGAL -lomp
CXXFLAGS=-std=c++17 -pedantic -Wall -O3 -fopenmp $(INCLUDES)

jet-test: jet-test.o jet-wrapper.o
	$(CXX) -o $@ $^ $(LIBRARIES)
