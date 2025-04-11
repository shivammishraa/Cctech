# Compiler Settings
CXX = g++
CXXFLAGS = -Wall -std=c++17 -Igeometry/include -D_DLL

# Directories
BIN_DIR = bin
DATA_DIR = data
SRC_DIR = application/src
GEOM_SRC_DIR = geometry/src
GEOM_INC_DIR = geometry/include

# Executable and DLL Targets
TARGET = $(BIN_DIR)/shape_plotter.exe
DLL_TARGET = $(BIN_DIR)/geometry.dll
LIBRARY = $(BIN_DIR)/libgeometry.a

# Object Files
OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%.o, $(wildcard $(SRC_DIR)/*.cpp)) \
      $(patsubst $(GEOM_SRC_DIR)/%.cpp, $(BIN_DIR)/%.o, $(wildcard $(GEOM_SRC_DIR)/*.cpp))

# Build the final executable
$(TARGET): $(OBJ) $(DLL_TARGET)
	$(CXX) $(OBJ) -o $(TARGET) -lm -L$(BIN_DIR) -lgeometry

# Compile the DLL
$(DLL_TARGET): $(BIN_DIR)/transform_utils.o $(BIN_DIR)/plot_utils.o $(BIN_DIR)/objToStl.o $(BIN_DIR)/stl_shape.o \
               $(BIN_DIR)/cuboid.o $(BIN_DIR)/cylinder.o $(BIN_DIR)/sphere.o $(BIN_DIR)/polyline.o $(BIN_DIR)/bezier.o \
               $(BIN_DIR)/triangle.o $(BIN_DIR)/polygon.o $(BIN_DIR)/line3D.o $(BIN_DIR)/scene.o
	$(CXX) -shared $(CXXFLAGS) -o $(DLL_TARGET) $^ -Wl,--out-implib,$(LIBRARY)

# Compile source files in application/src to object files in bin/
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile source files in geometry/src to object files in bin/
$(BIN_DIR)/%.o: $(GEOM_SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	@echo Cleaning up build files...
	-@del /q /f $(BIN_DIR)\*.o 2>nul
	-@del /q /f $(BIN_DIR)\*.exe 2>nul
	-@del /q /f $(BIN_DIR)\*.dll 2>nul
	-@del /q /f $(BIN_DIR)\*.a 2>nul
	-@del /q /f $(DATA_DIR)\*.dat 2>nul
	-@if exist $(BIN_DIR)\libgeometry.a del /q /f $(BIN_DIR)\libgeometry.a




# Build and run the project
run: $(TARGET)
	@echo "Starting program..."
	@$(BIN_DIR)/shape_plotter.exe
