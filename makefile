CC = gcc

all: featur res

featur: features.cpp
	$(CC) -c features.cpp

res: Result.cpp
	$(CC) -c Result.cpp

clean:
	$(RM) all
