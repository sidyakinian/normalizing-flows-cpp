CXX = g++
CXXFLAGS = -Wall -g -std=c++20
BUILD_DIR = build

TARGET = program

SRCS = $(wildcard src/*.cpp)

OBJS = $(addprefix $(BUILD_DIR)/, $(notdir $(SRCS:.cpp=.o)))

$(BUILD_DIR)/%.o: src/%.cpp $(wildcard src/*.h)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

.PHONY: clean

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
