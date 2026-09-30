#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

#include "../include/DLList.hpp"

void test_empty() {
    DLList<int> list;

    assert(list.empty());
    assert(list.size() == 0);
    assert(list.to_vector().empty());

    try {
        list.front();
        assert(false);
    } catch (const std::out_of_range&) {}

    try {
        list.back();
        assert(false);
    } catch (const std::out_of_range&) {}

    try {
        list.pop_front();
        assert(false);
    } catch (const std::out_of_range&) {}

    try {
        list.pop_back();
        assert(false);
    } catch (const std::out_of_range&) {}

    try {
        list.get(0);
        assert(false);
    } catch (const std::out_of_range&) {}

    try {
        list.get_recursive(0);
        assert(false);
    } catch (const std::out_of_range&) {}
}

void test_push_front() {
    DLList<int> list;

    list.push_front(3);
    list.push_front(2);
    list.push_front(1);

    assert(list.size() == 3);
    assert(!list.empty());

    assert(list.front() == 1);
    assert(list.back() == 3);

    assert(list.to_vector() ==
           std::vector<int>({1, 2, 3}));
}

void test_push_back() {
    DLList<int> list;

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    assert(list.size() == 3);

    assert(list.front() == 1);
    assert(list.back() == 3);

    assert(list.to_vector() ==
           std::vector<int>({1, 2, 3}));
}

void test_pop_front() {
    DLList<int> list;

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    assert(list.pop_front() == 1);
    assert(list.front() == 2);
    assert(list.back() == 3);
    assert(list.size() == 2);

    assert(list.pop_front() == 2);
    assert(list.pop_front() == 3);

    assert(list.empty());
}

void test_pop_back() {
    DLList<int> list;

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    assert(list.pop_back() == 3);
    assert(list.front() == 1);
    assert(list.back() == 2);
    assert(list.size() == 2);

    assert(list.pop_back() == 2);
    assert(list.pop_back() == 1);

    assert(list.empty());
}

void test_single_element_transitions() {
    DLList<int> list;

    // empty -> one -> empty through opposite ends
    list.push_front(42);

    assert(list.front() == 42);
    assert(list.back() == 42);

    assert(list.pop_back() == 42);

    assert(list.empty());
    assert(list.size() == 0);

    list.push_back(99);

    assert(list.front() == 99);
    assert(list.back() == 99);

    assert(list.pop_front() == 99);

    assert(list.empty());
}

void test_mixed_operations() {
    DLList<int> list;

    list.push_back(2);      // [2]
    list.push_front(1);     // [1, 2]
    list.push_back(3);      // [1, 2, 3]
    list.push_front(0);     // [0, 1, 2, 3]

    assert(list.to_vector() ==
           std::vector<int>({0, 1, 2, 3}));

    assert(list.pop_front() == 0); // [1, 2, 3]
    assert(list.pop_back() == 3);  // [1, 2]

    assert(list.to_vector() ==
           std::vector<int>({1, 2}));

    assert(list.front() == 1);
    assert(list.back() == 2);
}

void test_get() {
    DLList<int> list;

    for (int i = 0; i < 10; ++i) {
        list.push_back(i * 10);
    }

    for (std::size_t i = 0; i < 10; ++i) {
        assert(list.get(i) ==
               static_cast<int>(i * 10));
    }

    try {
        list.get(10);
        assert(false);
    } catch (const std::out_of_range&) {}
}

void test_get_recursive() {
    DLList<int> list;

    for (int i = 0; i < 10; ++i) {
        list.push_back(i * 10);
    }

    for (std::size_t i = 0; i < 10; ++i) {
        assert(list.get_recursive(i) ==
               static_cast<int>(i * 10));
    }

    try {
        list.get_recursive(10);
        assert(false);
    } catch (const std::out_of_range&) {}
}

void test_get_matches_recursive_get() {
    DLList<int> list;

    for (int i = 0; i < 100; ++i) {
        list.push_back(i);
    }

    for (std::size_t i = 0; i < list.size(); ++i) {
        assert(list.get(i) == list.get_recursive(i));
    }
}

void test_reference_access() {
    DLList<int> list;

    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    list.front() = 100;
    list.back() = 300;
    list.get(1) = 200;

    assert(list.to_vector() ==
           std::vector<int>({100, 200, 300}));

    list.get_recursive(1) = 999;

    assert(list.get(1) == 999);
}

void test_const_access() {
    DLList<int> temp;

    temp.push_back(10);
    temp.push_back(20);
    temp.push_back(30);

    const DLList<int>& list = temp;

    assert(list.front() == 10);
    assert(list.back() == 30);

    assert(list.get(0) == 10);
    assert(list.get(1) == 20);
    assert(list.get(2) == 30);

    assert(list.get_recursive(0) == 10);
    assert(list.get_recursive(1) == 20);
    assert(list.get_recursive(2) == 30);

    assert(list.size() == 3);
    assert(!list.empty());
}

void test_string() {
    DLList<std::string> list;

    list.push_back("B");
    list.push_front("A");
    list.push_back("C");

    assert(list.to_vector() ==
           std::vector<std::string>({"A", "B", "C"}));

    assert(list.front() == "A");
    assert(list.back() == "C");

    assert(list.pop_front() == "A");
    assert(list.pop_back() == "C");

    assert(list.front() == "B");
    assert(list.back() == "B");
}

int main() {
    static_assert(!std::is_copy_constructible_v<DLList<int>>);
    static_assert(!std::is_copy_assignable_v<DLList<int>>);

    test_empty();
    test_push_front();
    test_push_back();
    test_pop_front();
    test_pop_back();
    test_single_element_transitions();
    test_mixed_operations();
    test_get();
    test_get_recursive();
    test_get_matches_recursive_get();
    test_reference_access();
    test_const_access();
    test_string();

    std::cout << "All DLList tests passed.\n";

    return 0;
}