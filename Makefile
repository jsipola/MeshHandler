CC = g++
FLAGS = -std=c++11 -pthread
FILES = MeshServer.cpp MeshClass.cpp
EXE = meshserver

build: $(FILES)
	$(CC) $(FILES) -o $(EXE) $(FLAGS)

clean:
	rm -f *.o core

rebuild: clean build

run: $(EXE)
	./$(EXE) ${ARGS}


