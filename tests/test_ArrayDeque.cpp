#include "../include/ArrayDeque.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <vector>

void test_empty() {
    ArrayDeque<int> deque;

    assert(deque.empty());
    assert(deque.size() == 0);
    assert(deque.to_vector().empty());
}

void test_push_back() {
    ArrayDeque<int> deque;

    deque.push_back(1);
    deque.push_back(2);
    deque.push_back(3);

    assert(deque.size() == 3);
    assert(!deque.empty());

    assert(deque.front() == 1);
    assert(deque.back() == 3);

    assert(deque.get(0) == 1);
    assert(deque.get(1) == 2);
    assert(deque.get(2) == 3);

    assert((deque.to_vector() == std::vector<int>{1, 2, 3}));
}

void test_push_front() {
    ArrayDeque<int> deque;

    deque.push_front(1);
    deque.push_front(2);
    deque.push_front(3);

    assert(deque.size() == 3);

    assert(deque.front() == 3);
    assert(deque.back() == 1);

    assert((deque.to_vector() == std::vector<int>{3, 2, 1}));
}

void test_mixed_push() {
    ArrayDeque<int> deque;

    deque.push_back(2);
    deque.push_front(1);
    deque.push_back(3);
    deque.push_front(0);

    assert((deque.to_vector() == std::vector<int>{0, 1, 2, 3}));

    assert(deque.front() == 0);
    assert(deque.back() == 3);
}

void test_pop_front() {
    ArrayDeque<int> deque;

    deque.push_back(1);
    deque.push_back(2);
    deque.push_back(3);

    assert(deque.pop_front() == 1);
    assert(deque.pop_front() == 2);

    assert(deque.size() == 1);
    assert(deque.front() == 3);
    assert(deque.back() == 3);

    assert(deque.pop_front() == 3);
    assert(deque.empty());
}

void test_pop_back() {
    ArrayDeque<int> deque;

    deque.push_back(1);
    deque.push_back(2);
    deque.push_back(3);

    assert(deque.pop_back() == 3);
    assert(deque.pop_back() == 2);

    assert(deque.size() == 1);
    assert(deque.front() == 1);
    assert(deque.back() == 1);

    assert(deque.pop_back() == 1);
    assert(deque.empty());
}

void test_wrap_around() {
    ArrayDeque<int> deque;

    // Fill initial capacity.
    for (int i = 0; i < 8; ++i) {
        deque.push_back(i);
    }

    // Move front_ forward.
    for (int i = 0; i < 5; ++i) {
        assert(deque.pop_front() == i);
    }

    // These should wrap around to the beginning of the physical array.
    deque.push_back(8);
    deque.push_back(9);
    deque.push_back(10);
    deque.push_back(11);

    assert((deque.to_vector() ==
            std::vector<int>{5, 6, 7, 8, 9, 10, 11}));

    for (int i = 0; i < 7; ++i) {
        assert(deque.get(i) == i + 5);
    }
}

void test_wrap_around_front() {
    ArrayDeque<int> deque;

    deque.push_front(1);
    deque.push_front(2);
    deque.push_front(3);
    deque.push_front(4);

    assert((deque.to_vector() == std::vector<int>{4, 3, 2, 1}));

    assert(deque.pop_back() == 1);
    assert(deque.pop_back() == 2);

    deque.push_front(5);
    deque.push_front(6);

    assert((deque.to_vector() == std::vector<int>{6, 5, 4, 3}));
}

void test_resize_grow() {
    ArrayDeque<int> deque;

    // Initial capacity is 8, so this must trigger resize.
    for (int i = 0; i < 100; ++i) {
        deque.push_back(i);
    }

    assert(deque.size() == 100);

    for (int i = 0; i < 100; ++i) {
        assert(deque.get(i) == i);
    }

    assert(deque.front() == 0);
    assert(deque.back() == 99);
}

void test_resize_after_wrap() {
    ArrayDeque<int> deque;

    for (int i = 0; i < 8; ++i) {
        deque.push_back(i);
    }

    for (int i = 0; i < 4; ++i) {
        deque.pop_front();
    }

    for (int i = 8; i < 12; ++i) {
        deque.push_back(i);
    }

    // At this point the physical layout should already be wrapped.
    // This push forces resize.
    deque.push_back(12);

    assert((deque.to_vector() ==
            std::vector<int>{4, 5, 6, 7, 8, 9, 10, 11, 12}));
}

void test_shrink() {
    ArrayDeque<int> deque;

    for (int i = 0; i < 64; ++i) {
        deque.push_back(i);
    }

    for (int i = 0; i < 60; ++i) {
        assert(deque.pop_front() == i);
    }

    assert(deque.size() == 4);

    assert((deque.to_vector() == std::vector<int>{60, 61, 62, 63}));

    assert(deque.front() == 60);
    assert(deque.back() == 63);
}

void test_reuse_after_empty() {
    ArrayDeque<int> deque;

    for (int i = 0; i < 20; ++i) {
        deque.push_back(i);
    }

    for (int i = 0; i < 20; ++i) {
        assert(deque.pop_front() == i);
    }

    assert(deque.empty());

    deque.push_back(100);
    deque.push_front(99);
    deque.push_back(101);

    assert((deque.to_vector() == std::vector<int>{99, 100, 101}));
}

void test_reference_access() {
    ArrayDeque<int> deque;

    deque.push_back(1);
    deque.push_back(2);

    deque.front() = 10;
    deque.back() = 20;
    deque.get(0) = 100;

    assert((deque.to_vector() == std::vector<int>{100, 20}));
}

void test_const_access() {
    ArrayDeque<int> deque;

    deque.push_back(1);
    deque.push_back(2);
    deque.push_back(3);

    const ArrayDeque<int>& const_deque = deque;

    assert(const_deque.front() == 1);
    assert(const_deque.back() == 3);
    assert(const_deque.get(1) == 2);
    assert(const_deque.size() == 3);
    assert(!const_deque.empty());
}

void test_exceptions() {
    ArrayDeque<int> deque;

    bool threw = false;

    try {
        deque.front();
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    threw = false;
    try {
        deque.back();
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    threw = false;
    try {
        deque.pop_front();
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    threw = false;
    try {
        deque.pop_back();
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    deque.push_back(1);

    threw = false;
    try {
        deque.get(1);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);

    threw = false;
    try {
        deque.get(100);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);
}

void test_string() {
    ArrayDeque<std::string> deque;

    deque.push_back("B");
    deque.push_front("A");
    deque.push_back("C");

    assert(deque.front() == "A");
    assert(deque.back() == "C");

    assert((deque.to_vector() ==
            std::vector<std::string>{"A", "B", "C"}));
}

int main() {
    test_empty();

    test_push_back();
    test_push_front();
    test_mixed_push();

    test_pop_front();
    test_pop_back();

    test_wrap_around();
    test_wrap_around_front();

    test_resize_grow();
    test_resize_after_wrap();
    test_shrink();

    test_reuse_after_empty();

    test_reference_access();
    test_const_access();

    test_exceptions();
    test_string();

    std::cout << "All ArrayDeque tests passed!\n";

    return 0;
}