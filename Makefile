CXX=g++
CXXFLAGS=-ggdb `pkg-config --cflags --libs opencv`
all: featur res
featur:features.o
	$(CXX) $^ $(CXXFLAGS) -o $@
res:result.o 
	$(CXX) $^ -o $@
