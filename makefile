CXX = g++
CXXFLAGS = -c -std=c++11
SRCS = bTree_main.cpp bTree.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = bTree.h
EXEC = bTree

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) $<

clean:
	rm -rf *.o $(EXEC)

clear:
	clear
