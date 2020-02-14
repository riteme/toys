.DEFAULT_GOAL = run

COMMANDS = help all clean data run ranklist $(TARGETS)
.PHONY: $(COMMANDS)
help:
	@echo make [$(COMMANDS)]
	@echo "\t" help "\t\t" print this message.
	@echo "\t" all "\t\t" compile all implementations.
	@echo "\t" clean "\t\t" clean up objects and dependency files. Results are preserved.
	@echo "\t" data "\t\t" use utils/random-gen.py to generate new data.
	@echo "\t" [run] RUN= "\t" run all implementations.
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
HACK = uniform

# Compiler setting
CXX = g++
PYTHON = python
PYPY = pypy
DEBUG = 0

CXXFLAGS := -std=c++17 -I./headers/ -Wno-unused-result -Wno-char-subscripts
ifeq ($(DEBUG), 0)
CXXFLAGS += -O3 -flto -fno-stack-protector
else
CXX = clang++
CXXFLAGS += -g -fsanitize=undefined,address -Wall -Wextra -fno-inline
endif

GENFLAGS :=
ifeq ($(PRUFER), 1)
GENFLAGS += --prufer
endif

RUNFLAGS := -q -r $(repeat) -m $(max) -e $(maxe) -u $(HACK)
ifdef seed
RUNFLAGS += -s $(seed)
endif


SRCS := $(wildcard srcs/*.cpp)
IMPLS := $(wildcard impls/*.cpp)
SOURCES := $(SRCS) $(IMPLS)
DEPS := $(addprefix deps/, $(SOURCES:.cpp=.d))
TARGETS := $(basename $(notdir $(IMPLS)))
REAL_TARGETS := $(addprefix build/, $(TARGETS))
FRAMEWORK := $(addprefix build/, $(patsubst %.cpp, %.o, $(SRCS)))
LIB := build/libframework.a


ifneq ($(MAKECMDGOALS), clean)
-include $(DEPS)
endif


all: $(REAL_TARGETS)

$(REAL_TARGETS): build/% : $(LIB) build/impls/%.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(TARGETS): % : build/%

$(LIB): $(FRAMEWORK)
	ar -r $@ $^

build/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $< -c -o $@

deps/%.d: %.cpp
	@mkdir -p $(dir $@)
	$(PYTHON) utils/depend.py $(CXX) $< $(CXXFLAGS) > $@

clean:
	-rm build -rf
	-rm deps -rf

_mkdata := $(PYPY) utils/random-gen.py $(n) $(m) $(GENFLAGS) > data.in
data: ; $(_mkdata)
data.in: ; $(_mkdata)

RUN = $(TARGETS)
run: $(RUN) data.in
	@mkdir -p results
	@echo   FLAGS: $(RUNFLAGS)
	@echo TARGETS: $(RUN)
	@for t in $(RUN) ; do \
		echo Running \"$$t\" \> results/$$t.txt ; \
		./build/$$t $(RUNFLAGS) -o results/$$t.txt < data.in ; \
	done

ranklist:
	@$(PYTHON) utils/ranklist.py results