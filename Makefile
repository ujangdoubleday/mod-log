CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2
LDFLAGS = -lm
TARGET = run
SRC = main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
