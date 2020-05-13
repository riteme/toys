VERILATED_SRC = /usr/share/verilator/include/verilated.cpp
TOP_MODULE = CacheTop
TOP_MAKEFILE_NAME = V$(TOP_MODULE).mk
TOP_MAKEFILE = build/$(TOP_MAKEFILE_NAME)
VERILATED_LIB = build/V$(TOP_MODULE)__ALL.a

HEADERS = $(wildcard include/*.h)
LIBS = $(addprefix build/, main.o testbench.o verilated.o) $(VERILATED_LIB)
CXX = g++
CXX_INCLUDE = \
	-I /usr/share/verilator/include \
	-I /usr/share/verilator/include/vltstd \
	-I ./include \
	-I ./build
CXXFLAGS = -O2 -std=c++17 $(CXX_INCLUDE)

a.out: $(LIBS)
	$(CXX) $(CXXFLAGS) $(LIBS) -o a.out

build/verilated.o: $(VERILATED_SRC)
	$(CXX) $(CXXFLAGS) $< -c -o $@
build/testbench.o: testbench.cpp $(HEADERS) $(TOP_MAKEFILE)
	$(CXX) $(CXXFLAGS) $< -c -o $@
build/main.o: main.cpp include/testbench.h
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $< -c -o $@

$(VERILATED_LIB): $(TOP_MAKEFILE)
	cd build; make -f $(TOP_MAKEFILE_NAME)

$(TOP_MAKEFILE): src/*.sv src/*.vh
	verilator -Wall -cc -O2 -Mdir build -I./src src/$(TOP_MODULE).sv

.PHONY: clean impl test
clean:
	rm build -rf
	rm a.out -f

impl: TOP_MAKEFILE

test: a.out
	./a.out