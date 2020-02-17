.DEFAULT_GOAL = run

COMMANDS = help all clean dist-clean data run ranklist pack $(TARGETS)
.PHONY: $(COMMANDS)
help:
	@echo make [$(COMMANDS)]
	@echo "\t" help "\t\t" print this message.
	@echo "\t" all "\t\t" compile all implementations.
	@echo "\t" clean "\t\t" clean up all compiled objects.
	@echo "\t" dist-clean "\t" clean up objects and dependency files. Results are preserved.
	@echo "\t" data "\t\t" use utils/random-gen.py to generate new data.
	@echo "\t" [run] RUN= "\t" run all implementations.
	@echo "\t" ranklist "\t" list ranklist based on the results of last run.
	@echo "\t" pack TARNAME= "\t" pack results into tarfiles.
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
NOSPFA = 0

# Compiler setting
CXX = g++
PYTHON = python
PYPY = pypy
DEBUG = 0

CXXFLAGS := -std=c++17 -I./headers/ -Wno-unused-result -Wno-char-subscripts
ifeq ($(DEBUG), 0)
CXXFLAGS += -O3 -flto -fno-stack-protector -DNDEBUG
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
SPFAS := $(basename $(notdir $(wildcard impls/spfa*.cpp)))
REAL_TARGETS := $(addprefix build/, $(TARGETS))
FRAMEWORK := $(addprefix build/, $(patsubst %.cpp, %.o, $(SRCS)))
LIB := build/libframework.a


ifneq ($(MAKECMDGOALS), $(filter-out run all $(TARGETS), $(MAKECMDGOALS)))
-include $(DEPS)
else ifeq ($(MAKECMDGOALS), )
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

dist-clean: clean
	-rm deps -rf

_mkdata := $(PYPY) utils/random-gen.py $(n) $(m) $(GENFLAGS) > data.in
data: ; $(_mkdata)
data.in: ; $(_mkdata)

ifeq ($(filter-out run, $(MAKECMDGOALS)), )
ifeq ($(NOSPFA), 1)
RUN := $(filter-out $(SPFAS), $(TARGETS))
else
RUN := $(TARGETS)
endif
endif

run: $(RUN) data.in
	@mkdir -p results
	@echo   FLAGS: $(RUNFLAGS)
	@echo TARGETS: $(RUN)
	@for t in $(RUN) ; do \
		echo Running \"$$t\" \> results/$$t.txt ; \
		./build/$$t $(RUNFLAGS) -o results/$$t.txt < data.in ; \
	done
	tar cfv results/$(HACK).tar $(addprefix results/, $(addsuffix .txt, $(RUN)))

TARNAME =
ifeq ($(TARNAME), )
_RANK_TARGET = results
else
_RANK_TARGET = results/$(TARNAME).tar
endif

ranklist:
	@$(PYTHON) utils/ranklist.py $(_RANK_TARGET)

pack:
	tar cfv $(_RANK_TARGET) results/*.txt