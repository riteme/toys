-include $(DEPS)
 .DEFAULTGOAL: help

COMMANDS = help all clean data run ranklist $(TARGETS)
.PHONY: $(COMMANDS)
help:
	@echo make [$(COMMANDS)]
	@echo "\t" help "\t\t" print this message.
	@echo "\t" all "\t\t" compile all implementations.
	@echo "\t" clean "\t\t" clean up objects and dependency files. Results are preserved.
	@echo "\t" data "\t\t" use utils/random-gen.py to generate new data.
	@echo "\t" run "\t\t" run all implementations.
	@echo "\t" ranklist "\t" list ranklist based on the results of last run.
	@echo "\n" available implementations: "\n\t" $(TARGETS)

# for make data
n = 100000
m = 5000000
PRUFER = 0

# for make run
repeat = 4
max = 100
maxe = 10000000000
HACK = 0

# Compiler setting
CXX = g++
PYTHON = pypy
DEBUG = 0

CXXFLAGS := -std=c++17 -I./includes/ -Wno-unused-result -Wno-char-subscripts
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

SRCS := $(wildcard srcs/*.cpp)
IMPLS := $(wildcard impls/*.cpp)
SOURCES := $(SRCS) $(IMPLS)
DEPS := $(addprefix deps/, $(SOURCES:.cpp=.d))
TARGETS := $(basename $(notdir $(IMPLS)))
RESULTS := $(addprefix results/, $(addsuffix .txt, $(TARGETS)))
REAL_TARGETS := $(addprefix build/, $(TARGETS))
FRAMEWORK := $(addprefix build/, $(patsubst %.cpp, %.o, $(SRCS)))


all: $(REAL_TARGETS)

$(REAL_TARGETS): build/% : $(FRAMEWORK) build/impls/%.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TARGETS): $(REAL_TARGETS)

build/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $< -c -o $@

deps/%.d: %.cpp
	@mkdir -p $(dir $@)
	python utils/depend.py $(CXX) $< $(CXXFLAGS) -fsyntax-only > $@

clean:
	-rm build -rf
	-rm deps -rf

_mkdata := $(PYTHON) utils/random-gen.py $(n) $(m) $(GENFLAGS) > data.in
data: ; $(_mkdata)
data.in: ; $(_mkdata)

run: $(TARGETS) data.in
	@mkdir -p results
	@echo   FLAGS: $(RUNFLAGS)
	@echo TARGETS: $(TARGETS)
	@for t in $(TARGETS) ; do \
		echo Running \"$$t\" \> results/$$t.txt ; \
		./build/$$t $(RUNFLAGS) -o results/$$t.txt < data.in ; \
	done

ranklist: $(RESULTS)
	@python utils/ranklist.py results