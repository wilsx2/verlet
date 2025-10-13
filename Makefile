CC=g++
DEPS=-lsfml-graphics -lsfml-window -lsfml-system
EXE=verlet.exe
SRC=src/main.cpp src/Physics/PhysicsObject.cpp src/Physics/Solver.cpp src/Rendering/Renderer.cpp src/Utility/CollisionGrid.cpp

all: $(EXE)

$(EXE): $(SRC)
	$(CC) $(SRC) -o $(EXE) $(DEPS)

clean:
	rm $(EXE)


