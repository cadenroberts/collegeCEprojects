CXX = g++
CXXFLAGS = -Wall -std=c++11

OBJECTS = wordrange.o

wordrange: $(OBJECTS)
	$(CXX) -g $(CXXFLAGS) -o $@ $^

clean:
	rm -f *.o wordrange