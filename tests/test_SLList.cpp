#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>

#include "../include/SLList.hpp"

void test_empty() {
    SLList<int> list;

    assert(list.size() == 0);

    try {
        list.front();
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
}

void test_single_value_constructor() {
    SLList<int> list(42);

    assert(list.size() == 1);
    assert(list.front() == 42);
}

void test_push_front() {
    SLList<int> list;

    list.push_front(3);
    list.push_front(2);
    list.push_front(1);

    assert(list.size() == 3);
    assert(list.front() == 1);

    assert(list.pop_front() == 1);
    assert(list.pop_front() == 2);
    assert(list.pop_front() == 3);

    assert(list.size() == 0);
}

void test_push_back() {
    SLList<int> list;

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    assert(list.size() == 3);
    assert(list.front() == 1);

    assert(list.pop_front() == 1);
    assert(list.pop_front() == 2);
    assert(list.pop_front() == 3);

    assert(list.size() == 0);
}

void test_pop_back() {
    SLList<int> list;

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    assert(list.pop_back() == 3);
    assert(list.size() == 2);

    assert(list.pop_back() == 2);
    assert(list.size() == 1);

    assert(list.pop_back() == 1);
    assert(list.size() == 0);
}

void test_single_element_transitions() {
    SLList<int> list;

    list.push_front(10);
    assert(list.front() == 10);
    assert(list.pop_back() == 10);
    assert(list.size() == 0);

    list.push_back(20);
    assert(list.front() == 20);
    assert(list.pop_front() == 20);
    assert(list.size() == 0);
}

void test_mixed_operations() {
    SLList<int> list;

    list.push_back(2);      // [2]
    list.push_front(1);     // [1, 2]
    list.push_back(3);      // [1, 2, 3]
    list.push_front(0);     // [0, 1, 2, 3]

    assert(list.size() == 4);
    assert(list.front() == 0);

    assert(list.pop_front() == 0); // [1, 2, 3]
    assert(list.pop_back() == 3);  // [1, 2]

    assert(list.size() == 2);
    assert(list.front() == 1);

    assert(list.pop_front() == 1);
    assert(list.pop_back() == 2);

    assert(list.size() == 0);
}

void test_reference_front() {
    SLList<int> list(10);

    list.front() = 99;

    assert(list.front() == 99);
}

void test_const_front() {
    const SLList<int> list(123);

    assert(list.front() == 123);
    assert(list.size() == 1);
}

void test_string() {
    SLList<std::string> list;

    list.push_back("B");
    list.push_front("A");
    list.push_back("C");

    assert(list.front() == "A");
    assert(list.size() == 3);

    assert(list.pop_front() == "A");
    assert(list.pop_back() == "C");
    assert(list.pop_front() == "B");

    assert(list.size() == 0);
}

int main() {
    static_assert(!std::is_copy_constructible_v<SLList<int>>);
    static_assert(!std::is_copy_assignable_v<SLList<int>>);

    test_empty();
    test_single_value_constructor();
    test_push_front();
    test_push_back();
    test_pop_back();
    test_single_element_transitions();
    test_mixed_operations();
    test_reference_front();
    test_const_front();
    test_string();

    std::cout << "All SLList tests passed.\n";

    return 0;
}