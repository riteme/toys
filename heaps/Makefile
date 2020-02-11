CXX = g++
CXXFLAGS = -std=c++17 -O3 -flto
FRAMEWORK = main.o hackers.o shortest-path.o
BUILD_DIR = build

REPEAT = 3
WORKLOAD = 500000000

all: $(basename $(notdir $(wildcard impl/*.cpp)))

main.o hackers.o: framework.h hackers.h
shortest-path.o impl/*.o: framework.h

%: $(FRAMEWORK) impl/%.o
	$(CXX) $(CXXFLAGS) $^ -o $(BUILD_DIR)/$@

.PHONY: clean
clean:
	@rm *.o impl/*.o build -rf
	@mkdir build
	@touch build/.gitkeeper
