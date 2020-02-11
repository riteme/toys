CXX = g++
CXXFLAGS = -std=c++17 -O3 -flto

BUILD_DIR = build
DEPS_DIR = deps

SOURCES := $(wildcard *.cpp) $(wildcard impl/*.cpp)
TARGETS := $(basename $(notdir $(wildcard impl/*.cpp)))
FRAMEWORK := $(addprefix $(BUILD_DIR)/, $(patsubst %.cpp, %.o, $(wildcard *.cpp)))

REPEAT = 3
WORKLOAD = 500000000

.PHONY: all
all: $(TARGETS)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $< -c -o $@

-include $(addprefix $(DEPS_DIR)/, $(SOURCES:.cpp=.d))

$(DEPS_DIR)/%.d: %.cpp
	@mkdir -p $(dir $@)
	python depend.py $< > $@

$(TARGETS) : % : $(FRAMEWORK) $(BUILD_DIR)/impl/%.o
	$(CXX) $(CXXFLAGS) $^ -o $(BUILD_DIR)/$@

.PHONY: clean
clean:
	-rm build -rf
	-rm deps -rf
