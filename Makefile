CXX = g++
CXXFLAGS = -Wall -g -std=c++20
BUILD_DIR = build

TARGET = program

# Changed: Updated the wildcard search to look inside the 'src' directory for .cpp files
SRCS = $(wildcard src/*.cpp)

# No change here
OBJS = $(addprefix $(BUILD_DIR)/, $(notdir $(SRCS:.cpp=.o)))

# Changed: Updated the rule to look inside the 'src' directory for .cpp and .h files
$(BUILD_DIR)/%.o: src/%.cpp $(wildcard src/*.h)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# No change here
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

.PHONY: clean

# No change here
clean:
	rm -rf $(BUILD_DIR) $(TARGET)
