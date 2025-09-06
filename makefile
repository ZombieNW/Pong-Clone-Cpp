# Compiler
CXX = g++

# Project
TARGET = pong

# Source
SRC = src/main.cpp

# Raylib
RAYLIB_DIR = C:/raylib
INC = -I$(RAYLIB_DIR)/include
LIB = -L$(RAYLIB_DIR)/lib

# Linker flags
ifeq ($(OS),Windows_NT)
    LDFLAGS = -lraylib -lopengl32 -lgdi32 -lwinmm -mwindows
else
    LDFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif

# Build target
$(TARGET): $(SRC)
	$(CXX) $(SRC) -o $(TARGET) $(INC) $(LIB) $(LDFLAGS)

# Run target
run: $(TARGET)
	make
	./$(TARGET)

# Clean build files
clean:
	rm -f $(TARGET).exe $(TARGET)
