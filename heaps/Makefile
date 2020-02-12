n = 5
m = 10
repeat = 4
max = 100
maxe = 10000000000

PRUFER = 0
DEBUG = 0
HACK = 0

CXX = g++
PYTHON = pypy

CXXFLAGS := -std=c++17
ifeq ($(DEBUG), 0)
CXXFLAGS += -O3 -flto
else
CXX = clang++
CXXFLAGS += -g -fsanitize=undefined,address -Wall -Wextra -fno-inline
endif

GENFLAGS :=
ifeq ($(PRUFER), 1)
GENFLAGS += --prufer
endif

RUNFLAGS := -q -r $(repeat) -m $(max) -e $(maxe)
ifdef seed
RUNFLAGS += -s $(seed)
endif
ifeq ($(HACK), 1)
RUNFLAGS += -u
endif

SOURCES := $(wildcard *.cpp) $(wildcard impl/*.cpp)
TARGETS := $(basename $(notdir $(wildcard impl/*.cpp)))
REAL_TARGETS = $(addprefix build/, $(TARGETS))
FRAMEWORK := $(addprefix build/, $(patsubst %.cpp, %.o, $(wildcard *.cpp)))

REPEAT = 3
WORKLOAD = 500000000

.PHONY: all $(TARGETS)
all: $(REAL_TARGETS)

$(REAL_TARGETS) : build/% : $(FRAMEWORK) build/impl/%.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TARGETS): $(REAL_TARGETS)

build/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $< -c -o $@

-include $(addprefix deps/, $(SOURCES:.cpp=.d))

deps/%.d: %.cpp
	@mkdir -p $(dir $@)
	python depend.py $< > $@

.PHONY: clean data run
clean:
	-rm build -rf
	-rm deps -rf

mkdata := $(PYTHON) random-gen.py $(n) $(m) $(GENFLAGS) > data.in
data: ; $(mkdata)
data.in: ; $(mkdata)

run: $(TARGETS) data.in
	@mkdir -p results
	@echo FLAGS: $(RUNFLAGS)
	@echo TARGETS: $(TARGETS)
	@for t in $(TARGETS) ; do \
		echo Running \"$$t\" ... ; \
		./build/$$t $(RUNFLAGS) < data.in > results/$$t.txt ; \
	done
