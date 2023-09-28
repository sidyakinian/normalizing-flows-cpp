CXX = g++
CXXFLAGS = -Wall -g -std=c++20
BUILD_DIR = build

TARGET = program

SRCS = $(wildcard *.cpp)
OBJS = $(addprefix $(BUILD_DIR)/, $(SRCS:.cpp=.o))

$(BUILD_DIR)/%.o: %.cpp $(wildcard *.h)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

.PHONY: clean

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
