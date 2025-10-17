# Compiler settings
CC := g++
CFLAGS := -O3 -march=native
DEPS := -lsfml-graphics -lsfml-window -lsfml-system

# Directories
BUILD_DIR := build
SRC_DIR := src
TEST_DIR := tests
RES_DIR := res

# Files
SRCS := $(shell find $(SRC_DIR) -name "*.cpp")
TESTS := $(shell find $(TEST_DIR) -name "*.cpp")
EXES := $(foreach exe, $(TESTS:%.cpp=%.exe), $(BUILD_DIR)/$(notdir $(exe))) # Create exe for each test

# Targets
all: $(EXES)
test-sim: clean all
	cd build && ./sim.exe

$(BUILD_DIR)/%.exe: $(TEST_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	cp -r $(RES_DIR) $(BUILD_DIR)/$(RES_DIR)
	$(CC) $(CFLAGS) $^ $(SRCS) -o $@ $(DEPS)

clean:
	rm -r $(BUILD_DIR)