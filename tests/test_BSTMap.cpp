#include "../include/BSTMap.hpp"

#include <cstddef>
#include <iostream>
#include <map>
#include <optional>
#include <random>
#include <set>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

static_assert(!std::is_copy_constructible_v<BSTMap<int, std::string>>);
static_assert(!std::is_copy_assignable_v<BSTMap<int, std::string>>);

void check(bool condition, const char* message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

// Compare all observable BSTMap state against a reference std::map.
void expect_state(const BSTMap<int, std::string>& bst,
                  const std::map<int, std::string>& reference) {
    check(bst.size() == reference.size(), "size() disagrees with std::map");

    std::vector<int> expected_keys;
    std::set<int> expected_key_set;
    for (const auto& [key, value] : reference) {
        expected_keys.push_back(key);
        expected_key_set.insert(key);
        check(bst.containsKey(key), "containsKey() missed an existing key");
        const std::string* got = bst.get(key);
        check(got != nullptr, "get() returned nullptr for an existing key");
        check(*got == value, "get() returned a value paired with the wrong key");
    }

    check(bst.inorder() == expected_keys, "inorder() must return sorted keys");
    check(bst.keySet() == expected_key_set, "keySet() contents are incorrect");
}

void test_empty() {
    BSTMap<int, std::string> bst;
    expect_state(bst, {});
    check(!bst.containsKey(10), "empty map contains a key");
    check(bst.get(10) == nullptr, "empty get() should return nullptr");
    check(!bst.remove(10).has_value(), "empty remove() should return nullopt");
    bst.clear();
    bst.clear();
    expect_state(bst, {});
}

void test_insert_and_lookup() {
    BSTMap<int, std::string> bst;
    std::map<int, std::string> reference;

    for (auto [key, value] : std::vector<std::pair<int, std::string>>{
             {8, "eight"}, {3, "three"}, {12, "twelve"},
             {1, "one"}, {6, "six"}, {10, "ten"},
             {15, "fifteen"}, {4, "four"}, {7, "seven"}}) {
        bst.put(key, value);
        reference[key] = value;
    }

    expect_state(bst, reference);
    for (int missing : {-10, 0, 2, 5, 9, 11, 13, 16}) {
        check(!bst.containsKey(missing), "containsKey() returned true for a missing key");
        check(bst.get(missing) == nullptr, "get() should be nullptr for a missing key");
    }
}

void test_duplicate_key_updates() {
    BSTMap<int, std::string> bst;
    bst.put(5, "old");
    bst.put(2, "other");
    bst.put(5, "new");
    bst.put(5, "newer");
    expect_state(bst, {{2, "other"}, {5, "newer"}});
    check(bst.size() == 2, "updating a key must not grow the map");
}

void test_duplicate_values_allowed() {
    BSTMap<int, std::string> bst;
    bst.put(1, "same");
    bst.put(2, "same");
    bst.put(3, "same");
    expect_state(bst, {{1, "same"}, {2, "same"}, {3, "same"}});
    bst.put(2, "changed");
    expect_state(bst, {{1, "same"}, {2, "changed"}, {3, "same"}});
}

void test_remove_leaf_and_missing() {
    BSTMap<int, std::string> bst;
    for (int key : {8, 3, 12, 1, 6}) {
        bst.put(key, "v" + std::to_string(key));
    }
    check(bst.remove(1) == std::optional<std::string>{"v1"},
          "removing a leaf should return its old value");
    expect_state(bst, {{3, "v3"}, {6, "v6"}, {8, "v8"}, {12, "v12"}});
    check(!bst.remove(999).has_value(), "removing a missing key must return nullopt");
    expect_state(bst, {{3, "v3"}, {6, "v6"}, {8, "v8"}, {12, "v12"}});
}

void test_remove_one_child() {
    {
        BSTMap<int, std::string> bst;
        bst.put(8, "eight");
        bst.put(3, "three");
        bst.put(1, "one");
        check(bst.remove(3) == std::optional<std::string>{"three"},
              "remove() with left child returned wrong value");
        expect_state(bst, {{1, "one"}, {8, "eight"}});
    }
    {
        BSTMap<int, std::string> bst;
        bst.put(8, "eight");
        bst.put(3, "three");
        bst.put(6, "six");
        check(bst.remove(3) == std::optional<std::string>{"three"},
              "remove() with right child returned wrong value");
        expect_state(bst, {{6, "six"}, {8, "eight"}});
    }
}

void test_remove_root_cases() {
    {
        BSTMap<int, std::string> bst;
        bst.put(4, "four");
        check(bst.remove(4) == std::optional<std::string>{"four"},
              "removing the only node returned wrong value");
        expect_state(bst, {});
        bst.put(9, "nine");
        expect_state(bst, {{9, "nine"}});
    }
    {
        BSTMap<int, std::string> bst;
        bst.put(4, "four");
        bst.put(2, "two");
        check(bst.remove(4) == std::optional<std::string>{"four"},
              "removing root with left child failed");
        expect_state(bst, {{2, "two"}});
    }
    {
        BSTMap<int, std::string> bst;
        bst.put(4, "four");
        bst.put(6, "six");
        check(bst.remove(4) == std::optional<std::string>{"four"},
              "removing root with right child failed");
        expect_state(bst, {{6, "six"}});
    }
}

void test_remove_two_children_direct_successor() {
    BSTMap<int, std::string> bst;
    for (int key : {8, 3, 12, 15}) {
        bst.put(key, "v" + std::to_string(key));
    }
    // Successor of 8 is its direct right child 12, which has a right child.
    check(bst.remove(8) == std::optional<std::string>{"v8"},
          "two-child root deletion returned wrong original value");
    expect_state(bst, {{3, "v3"}, {12, "v12"}, {15, "v15"}});
}

void test_remove_two_children_deep_successor() {
    BSTMap<int, std::string> bst;
    for (int key : {50, 30, 90, 70, 110, 60, 80, 65}) {
        bst.put(key, "v" + std::to_string(key));
    }
    // Successor of 50 is 60 (not a direct child), and 60 has right child 65.
    check(bst.remove(50) == std::optional<std::string>{"v50"},
          "deep successor deletion returned wrong original value");
    expect_state(bst, {{30, "v30"}, {60, "v60"}, {65, "v65"},
                       {70, "v70"}, {80, "v80"}, {90, "v90"}, {110, "v110"}});
    check(bst.remove(70) == std::optional<std::string>{"v70"},
          "subsequent interior deletion returned wrong value");
    expect_state(bst, {{30, "v30"}, {60, "v60"}, {65, "v65"},
                       {80, "v80"}, {90, "v90"}, {110, "v110"}});
}

void test_clear_and_reuse() {
    BSTMap<int, std::string> bst;
    for (int i = 0; i < 50; ++i) {
        bst.put(i, "v" + std::to_string(i));
    }
    bst.clear();
    expect_state(bst, {});
    check(bst.get(25) == nullptr, "get() returned stale data after clear()");
    bst.clear();
    bst.put(7, "seven");
    bst.put(2, "two");
    expect_state(bst, {{2, "two"}, {7, "seven"}});
}

void test_string_keys() {
    BSTMap<std::string, int> bst;
    bst.put("banana", 2);
    bst.put("apple", 1);
    bst.put("cherry", 3);
    bst.put("banana", 20);
    check(bst.size() == 3, "string key updates changed size");
    check(bst.inorder() == std::vector<std::string>({"apple", "banana", "cherry"}),
          "string keys are not sorted correctly");
    check(bst.keySet() == std::set<std::string>({"apple", "banana", "cherry"}),
          "keySet() failed with string keys");
    check(bst.get("banana") != nullptr && *bst.get("banana") == 20,
          "string key value update failed");
    check(bst.remove("banana") == std::optional<int>{20},
          "string key removal failed");
    check(bst.get("banana") == nullptr, "deleted string key still present");
}

// A legitimate key type need only implement operator<, not operator> or operator==.
struct LessOnlyKey {
    int id;
    bool operator<(const LessOnlyKey& other) const { return id < other.id; }
};

struct Payload {
    std::string name;
    int score;
};

void test_generic_types() {
    BSTMap<LessOnlyKey, Payload> bst;
    bst.put({5}, {"five", 50});
    bst.put({2}, {"two", 20});
    bst.put({8}, {"eight", 80});
    bst.put({5}, {"updated", 500});
    check(bst.size() == 3, "generic map size incorrect");
    check(bst.containsKey({2}), "generic key lookup failed");
    const Payload* value = bst.get({5});
    check(value != nullptr && value->name == "updated" && value->score == 500,
          "generic value update failed");
    auto removed = bst.remove({5});
    check(removed.has_value() && removed->name == "updated" && removed->score == 500,
          "generic value removal failed");
    check(bst.inorder().size() == 2 && bst.inorder()[0].id == 2 && bst.inorder()[1].id == 8,
          "generic key inorder traversal failed");
}

void test_randomized_against_std_map() {
    BSTMap<int, std::string> bst;
    std::map<int, std::string> reference;
    std::mt19937 rng(0xB57u);
    std::uniform_int_distribution<int> key_dist(-100, 100);
    std::uniform_int_distribution<int> op_dist(0, 4);

    for (int step = 0; step < 2500; ++step) {
        const int key = key_dist(rng);
        const int op = op_dist(rng);
        if (op <= 1) {
            const std::string value = "value_" + std::to_string(step);
            bst.put(key, value);
            reference[key] = value;
        } else if (op == 2) {
            const auto it = reference.find(key);
            const std::optional<std::string> expected =
                it == reference.end() ? std::nullopt
                                      : std::optional<std::string>{it->second};
            check(bst.remove(key) == expected,
                  "random remove() returned incorrect optional value");
            reference.erase(key);
        } else {
            const auto it = reference.find(key);
            check(bst.containsKey(key) == (it != reference.end()),
                  "random containsKey() disagrees with std::map");
            const std::string* got = bst.get(key);
            if (it == reference.end()) {
                check(got == nullptr, "random get() returned non-null for missing key");
            } else {
                check(got != nullptr && *got == it->second,
                      "random get() returned wrong value");
            }
        }
        expect_state(bst, reference);
    }

    // Delete everything and verify the map can be reused.
    for (const auto& [key, value] : reference) {
        check(bst.remove(key) == std::optional<std::string>{value},
              "draining random map returned wrong value");
    }
    expect_state(bst, {});
    bst.put(42, "reused");
    expect_state(bst, {{42, "reused"}});
}

int main() {
    try {
        test_empty();
        test_insert_and_lookup();
        test_duplicate_key_updates();
        test_duplicate_values_allowed();
        test_remove_leaf_and_missing();
        test_remove_one_child();
        test_remove_root_cases();
        test_remove_two_children_direct_successor();
        test_remove_two_children_deep_successor();
        test_clear_and_reuse();
        test_string_keys();
        test_generic_types();
        test_randomized_against_std_map();
        std::cout << "All BSTMap tests passed!\n";
    } catch (const std::exception& ex) {
        std::cerr << "BSTMap TEST FAILED: " << ex.what() << '\n';
        return 1;
    }
}