VDATAPATH_LIB = build/VDatapath__ALL.a
VERILATED_SRC = /usr/share/verilator/include/verilated.cpp
LIBS = build/testbench.o build/verilated.o $(VDATAPATH_LIB)
CXX = g++
CXXFLAGS = -g -std=c++17 -I /usr/share/verilator/include -I /usr/share/verilator/include/vltstd -I ./build

a.out: $(LIBS)
	$(CXX) $(CXXFLAGS) $(LIBS)

build/verilated.o: $(VERILATED_SRC)
	$(CXX) $(CXXFLAGS) $< -c -o $@

build/testbench.o: testbench.cpp testbench.h literal.h build/VDatapath.mk
	$(CXX) $(CXXFLAGS) $< -c -o $@

$(VDATAPATH_LIB): build/VDatapath.mk
	cd build; make -f VDatapath.mk

build/VDatapath.mk: src/*.sv src/*.vh
	verilator -Wall -cc -Mdir build -I./src src/Datapath.sv

.PHONY: clean verilator
clean:
	rm build -rf
	rm a.out -f

verilator: build/VDatapath.mk