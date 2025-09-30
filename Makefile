CC=g++
DEPS=-lsfml-graphics -lsfml-window -lsfml-system
EXE=verlet.exe
SRC=src/main.cpp

all: $(EXE)

$(EXE): $(SRC)
	$(CC) $(SRC) -o $(EXE) $(DEPS)

clean:
	rm $(EXE)


