CXX := g++

CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic -g
SANFLAGS := -fsanitize=address,undefined -fno-omit-frame-pointer

BUILD_DIR := build
TEST_DIR := tests
INCLUDE_DIR := include

SLLIST_TEST := $(BUILD_DIR)/test_SLList
DLLIST_TEST := $(BUILD_DIR)/test_DLList

.PHONY: all test clean sanitize

all: $(SLLIST_TEST) $(DLLIST_TEST)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(SLLIST_TEST): $(TEST_DIR)/test_SLList.cpp $(INCLUDE_DIR)/SLList.hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(DLLIST_TEST): $(TEST_DIR)/test_DLList.cpp $(INCLUDE_DIR)/DLList.hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

test: all
	./$(SLLIST_TEST)
	./$(DLLIST_TEST)

sanitize: CXXFLAGS += $(SANFLAGS)
sanitize: clean all
	./$(SLLIST_TEST)
	./$(DLLIST_TEST)

clean:
	rm -rf $(BUILD_DIR)