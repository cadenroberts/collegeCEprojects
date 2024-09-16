CXX = g++
CXXFLAGS = -std=c++11 -Wall

OBJECTS = shakespeareQuery.o

bard: $(OBJECTS)
			$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

shakespeareQuery.o: shakespeareQuery.cpp
		$(CXX) $(CXXFLAGS) -c shakespeareQuery.cpp

clean:
	rm $(OBJECTS) bard