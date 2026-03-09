CXX = g++
CXXFLAGS = -Wall -std=c++17
TARGET = A2

all: $(TARGET)

$(TARGET): main.o dsw.o
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.o dsw.o

main.o: main.cpp dsw.h
	$(CXX) $(CXXFLAGS) -c main.cpp

dsw.o: dsw.cpp dsw.h
	$(CXX) $(CXXFLAGS) -c dsw.cpp

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f *.o $(TARGET)