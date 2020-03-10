VCORE_LIB = build/VCore__ALL.a
VERILATED_SRC = /usr/share/verilator/include/verilated.cpp
LIBS = build/testbench.o build/verilated.o $(VCORE_LIB)
CXXFLAGS = -O3 -std=c++17 -I /usr/share/verilator/include -I ./build

a.out: $(LIBS)
	g++ $(CXXFLAGS) $(LIBS)

build/verilated.o: $(VERILATED_SRC)
	g++ $(CXXFLAGS) $< -c -o $@

build/testbench.o: testbench.cpp build/VCore.mk
	g++ $(CXXFLAGS) $< -c -o $@

$(VCORE_LIB): build/VCore.mk
	cd build; make -f VCore.mk

build/VCore.mk: src/*.sv
	verilator -Wall -cc -Mdir build -I./src src/Core.sv -O3

.PHONY: clean
clean:
	rm build -rf
	rm a.out -f