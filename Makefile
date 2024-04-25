CXX=clang++
# Directory structure
SRC_DIR=src
BUILD_DIR=./build

# Compiler flags
CXXFLAGS=-std=c++11 -O2 -march=native

# Find all the C++ files in src and test directories
LIBS=$(SRC_DIR)/complex.cpp $(SRC_DIR)/vec_op.cpp $(SRC_DIR)/qubit.cpp $(SRC_DIR)/unitary.cpp $(SRC_DIR)/circuit.cpp $(SRC_DIR)/err.cpp

SRCS=$(LIBS) $(SRC_DIR)/main.cpp
TSTS=$(LIBS) $(SRC_DIR)/test.cpp

# Create object file names by replacing .cpp with .o
OBJS=$(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
TST_OBJS=$(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(TSTS))

# Targets
TARGET=$(BUILD_DIR)/solve
TST_TARGET=$(BUILD_DIR)/test


$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TST_TARGET): $(TST_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build targets
all: $(TARGET)

run: $(TARGET)
	$(TARGET)

test: $(TST_TARGET)
	$(TST_TARGET)

# Clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: clean all test
