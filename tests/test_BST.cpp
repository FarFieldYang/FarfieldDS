#include "../include/BST.hpp"

#include <cstddef>
#include <iostream>
#include <random>
#include <set>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

static_assert(!std::is_copy_constructible_v<BST<int>>);
static_assert(!std::is_copy_assignable_v<BST<int>>);

void check(bool condition, const char* message) {
    if (!condition)
        throw std::runtime_error(message);
}

template <typename T>
void expect_contents(const BST<T>& bst,
                     const std::vector<T>& expected,
                     const char* message) {
    check(bst.inorder() == expected, message);
    check(bst.size() == expected.size(), message);
    check(bst.empty() == expected.empty(), message);
    for (const auto& value : expected)
        check(bst.contains(value), message);
}

void test_empty() {
    BST<int> bst;
    expect_contents(bst, std::vector<int>{}, "Empty BST state is incorrect");
    check(!bst.contains(42), "Empty BST must not contain 42");
    bst.remove(42);
    expect_contents(bst, std::vector<int>{}, "Removing from empty BST changed it");
    bst.clear();
    bst.clear();
    expect_contents(bst, std::vector<int>{}, "Clearing empty BST changed it");
}

void test_insert_contains_and_inorder() {
    BST<int> bst;
    for (int value : {8, 3, 12, 1, 6, 10, 15, 4, 7})
        bst.insert(value);

    expect_contents(bst, std::vector<int>{1, 3, 4, 6, 7, 8, 10, 12, 15},
                    "Insertion or inorder traversal is incorrect");
    for (int value : {0, 2, 5, 9, 11, 13, 16})
        check(!bst.contains(value), "contains() returned true for missing value");

    const BST<int>& const_bst = bst;
    check(const_bst.contains(10), "const contains() failed");
    check(const_bst.inorder().front() == 1, "const inorder() failed");
}

void test_duplicate_insert() {
    BST<int> bst;
    for (int value : {5, 3, 7, 5, 3, 7, 5})
        bst.insert(value);
    expect_contents(bst, std::vector<int>{3, 5, 7},
                    "Duplicate insertion must not change the BST");
}

void test_remove_leaf_and_missing() {
    BST<int> bst;
    for (int value : {8, 3, 12, 1, 6})
        bst.insert(value);

    bst.remove(1);
    expect_contents(bst, std::vector<int>{3, 6, 8, 12},
                    "Removing a leaf failed");
    check(!bst.contains(1), "Removed leaf is still present");
    bst.remove(999);
    bst.remove(-999);
    expect_contents(bst, std::vector<int>{3, 6, 8, 12},
                    "Removing absent values changed BST size or elements");
}

void test_remove_single_child() {
    BST<int> left_child;
    for (int value : {10, 5, 3})
        left_child.insert(value);
    left_child.remove(5);
    expect_contents(left_child, std::vector<int>{3, 10},
                    "Removing node with left child failed");

    BST<int> right_child;
    for (int value : {10, 5, 7})
        right_child.insert(value);
    right_child.remove(5);
    expect_contents(right_child, std::vector<int>{7, 10},
                    "Removing node with right child failed");

    BST<int> root_left;
    root_left.insert(10);
    root_left.insert(4);
    root_left.remove(10);
    expect_contents(root_left, std::vector<int>{4},
                    "Removing root with left child failed");

    BST<int> root_right;
    root_right.insert(10);
    root_right.insert(20);
    root_right.remove(10);
    expect_contents(root_right, std::vector<int>{20},
                    "Removing root with right child failed");
}

void test_remove_two_children_direct_successor() {
    BST<int> bst;
    for (int value : {8, 3, 12, 15})
        bst.insert(value);
    bst.remove(8); // successor is the direct right child (12)
    expect_contents(bst, std::vector<int>{3, 12, 15},
                    "Two-child removal with direct successor failed");
    check(!bst.contains(8), "Deleted root is still present");
}

void test_remove_two_children_deep_successor() {
    BST<int> bst;
    for (int value : {8, 3, 20, 12, 25, 10, 15, 11})
        bst.insert(value);
    bst.remove(8); // successor 10 itself has a right child 11
    expect_contents(bst, std::vector<int>{3, 10, 11, 12, 15, 20, 25},
                    "Two-child removal with deep successor failed");
    check(!bst.contains(8), "Deleted value remains after successor replacement");
}

void test_remove_internal_two_children() {
    BST<int> bst;
    for (int value : {10, 5, 15, 3, 7, 12, 18, 6, 8})
        bst.insert(value);
    bst.remove(5);
    expect_contents(bst, std::vector<int>{3, 6, 7, 8, 10, 12, 15, 18},
                    "Removing internal node with two children failed");
}

void test_remove_to_empty() {
    BST<int> bst;
    bst.insert(42);
    bst.remove(42);
    expect_contents(bst, std::vector<int>{}, "Removing only node failed");
    bst.remove(42);
    expect_contents(bst, std::vector<int>{}, "Removing absent node underflowed size");

    for (int value : {5, 2, 8, 1, 3, 7, 9})
        bst.insert(value);
    for (int value : {5, 2, 8, 1, 3, 7, 9})
        bst.remove(value);
    expect_contents(bst, std::vector<int>{}, "BST did not become empty");
}

void test_clear_and_reuse() {
    BST<int> bst;
    for (int i = 0; i < 100; ++i)
        bst.insert(i);
    check(bst.size() == 100, "Insertion count before clear is wrong");
    bst.clear();
    expect_contents(bst, std::vector<int>{}, "clear() failed");
    bst.clear();
    bst.insert(6);
    bst.insert(-2);
    expect_contents(bst, std::vector<int>{-2, 6},
                    "Reusing a cleared BST failed");
}

void test_degenerate_trees() {
    BST<int> ascending;
    BST<int> descending;
    for (int i = 0; i < 128; ++i) {
        ascending.insert(i);
        descending.insert(127 - i);
    }
    std::vector<int> expected;
    for (int i = 0; i < 128; ++i)
        expected.push_back(i);
    expect_contents(ascending, expected, "Ascending inserts failed");
    expect_contents(descending, expected, "Descending inserts failed");
    for (int i = 0; i < 128; ++i) {
        ascending.remove(i);
        descending.remove(i);
    }
    expect_contents(ascending, std::vector<int>{}, "Ascending tree delete-all failed");
    expect_contents(descending, std::vector<int>{}, "Descending tree delete-all failed");
}

void test_string_values() {
    BST<std::string> bst;
    for (const char* value : {"pear", "apple", "orange", "banana"})
        bst.insert(value);
    expect_contents(bst, std::vector<std::string>{"apple", "banana", "orange", "pear"},
                    "String BST insert/inorder failed");
    bst.remove("pear");
    bst.remove("apple");
    expect_contents(bst, std::vector<std::string>{"banana", "orange"},
                    "String BST deletion failed");
}

void test_randomized_against_std_set() {
    BST<int> bst;
    std::set<int> reference;
    std::mt19937 rng(20261008);
    std::uniform_int_distribution<int> values(-100, 100);
    std::uniform_int_distribution<int> actions(0, 2);

    for (int step = 0; step < 1500; ++step) {
        int value = values(rng);
        int action = actions(rng);
        if (action == 0) {
            bst.insert(value);
            reference.insert(value);
        } else if (action == 1) {
            bst.remove(value);
            reference.erase(value);
        } else {
            check(bst.contains(value) == reference.contains(value),
                  "Randomized contains() disagrees with std::set");
        }

        const std::vector<int> expected(reference.begin(), reference.end());
        expect_contents(bst, expected, "Randomized BST differs from std::set");
    }
}

int main() {
    try {
        test_empty();
        test_insert_contains_and_inorder();
        test_duplicate_insert();
        test_remove_leaf_and_missing();
        test_remove_single_child();
        test_remove_two_children_direct_successor();
        test_remove_two_children_deep_successor();
        test_remove_internal_two_children();
        test_remove_to_empty();
        test_clear_and_reuse();
        test_degenerate_trees();
        test_string_values();
        test_randomized_against_std_set();

        std::cout << "All BST tests passed!\n";
    } catch (const std::exception& e) {
        std::cerr << "BST TEST FAILED: " << e.what() << '\n';
        return 1;
    }
    return 0;
}