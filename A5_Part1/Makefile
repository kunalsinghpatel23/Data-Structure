CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

TARGET = binomial
OBJS = main.o binomial.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp binomial.h
	$(CXX) $(CXXFLAGS) -c main.cpp

binomial.o: binomial.cpp binomial.h
	$(CXX) $(CXXFLAGS) -c binomial.cpp

run: $(TARGET)
	./$(TARGET).exe

clean:
	rm -f $(TARGET) $(TARGET).exe $(OBJS)