CC=g++
CFLAGS=-O3 -march=native
DEPS=-lsfml-graphics -lsfml-window -lsfml-system
EXE=verlet.exe
SRC=src/main.cpp src/Physics/PhysicsObject.cpp src/Physics/Solver.cpp src/Rendering/Renderer.cpp src/Utility/CollisionGrid.cpp src/Utility/ThreadPool.cpp

all: $(EXE)

$(EXE): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(EXE) $(DEPS)

clean:
	rm $(EXE)


