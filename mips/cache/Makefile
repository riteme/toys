VERILATED_LIB = build/VCacheTop__ALL.a
VERILATED_SRC = /usr/share/verilator/include/verilated.cpp
HEADERS = $(wildcard include/*.h)
LIBS = build/testbench.o build/verilated.o $(VERILATED_LIB)
CXX = g++
CXXFLAGS = -g -std=c++17 -I /usr/share/verilator/include -I /usr/share/verilator/include/vltstd -I ./build

a.out: $(LIBS)
	$(CXX) $(CXXFLAGS) $(LIBS) -o a.out

build/verilated.o: $(VERILATED_SRC)
	$(CXX) $(CXXFLAGS) $< -c -o $@

build/testbench.o: testbench.cpp $(HEADERS) build/VCacheTop.mk
	$(CXX) $(CXXFLAGS) $< -c -o $@

$(VERILATED_LIB): build/VCacheTop.mk
	cd build; make -f VCacheTop.mk

build/VCacheTop.mk: src/*.sv src/*.vh
	verilator -Wall -cc -Mdir build -I./src src/CacheTop.sv

.PHONY: clean impl test
clean:
	rm build -rf
	rm a.out -f

impl: build/VCacheTop.mk

test: a.out
	./a.out