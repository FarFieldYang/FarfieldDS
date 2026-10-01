CXX := g++

CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic -g
SANFLAGS := -fsanitize=address,undefined -fno-omit-frame-pointer
BENCHFLAGS := -std=c++20 -O3 -DNDEBUG

BUILD_DIR := build
TEST_DIR := tests
INCLUDE_DIR := include

SLLIST_TEST := $(BUILD_DIR)/test_SLList
DLLIST_TEST := $(BUILD_DIR)/test_DLList
ARRAYDEQUE_TEST := $(BUILD_DIR)/test_ArrayDeque
UNIONFIND_TEST := $(BUILD_DIR)/test_UnionFind
UNIONFIND_BENCH := $(BUILD_DIR)/test_UnionFind_bench

.PHONY: all test sanitize bench clean

all: $(SLLIST_TEST) $(DLLIST_TEST) $(ARRAYDEQUE_TEST) $(UNIONFIND_TEST)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(SLLIST_TEST): $(TEST_DIR)/test_SLList.cpp $(INCLUDE_DIR)/SLList.hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(DLLIST_TEST): $(TEST_DIR)/test_DLList.cpp $(INCLUDE_DIR)/DLList.hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(ARRAYDEQUE_TEST): $(TEST_DIR)/test_ArrayDeque.cpp $(INCLUDE_DIR)/ArrayDeque.hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(UNIONFIND_TEST): $(TEST_DIR)/test_UnionFind.cpp $(INCLUDE_DIR)/UnionFind.hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(UNIONFIND_BENCH): $(TEST_DIR)/test_UnionFind.cpp $(INCLUDE_DIR)/UnionFind.hpp | $(BUILD_DIR)
	$(CXX) $(BENCHFLAGS) $< -o $@

test: all
	./$(SLLIST_TEST)
	./$(DLLIST_TEST)
	./$(ARRAYDEQUE_TEST)
	./$(UNIONFIND_TEST)

sanitize: CXXFLAGS += $(SANFLAGS)
sanitize: clean all
	./$(SLLIST_TEST)
	./$(DLLIST_TEST)
	./$(ARRAYDEQUE_TEST)
	./$(UNIONFIND_TEST)

bench: $(UNIONFIND_BENCH)
	./$(UNIONFIND_BENCH)

clean:
	rm -rf $(BUILD_DIR)