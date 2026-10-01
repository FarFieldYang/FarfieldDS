#include "../include/UnionFind.hpp"

#include <chrono>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <stdexcept>

using Clock = std::chrono::steady_clock;

void check(bool condition, const char* message) {
    if (!condition) {
        throw std::runtime_error(message);
    }
}

void test_initial_state() {
    UnionFind uf(100);

    for (UnionFind::Id i = 0; i < 100; ++i) {
        check(uf.find(i) == i,
              "Initial find failed");

        check(uf.size_of(i) == 1,
              "Initial size failed");
    }
}

void test_basic_union() {
    UnionFind uf(10);

    uf.unite(0, 1);

    check(uf.connected(0, 1),
          "0 and 1 should be connected");

    check(uf.size_of(0) == 2,
          "Set size should be 2");

    check(uf.size_of(1) == 2,
          "Set size should be 2");
}

void test_multiple_sets() {
    UnionFind uf(20);

    uf.unite(0, 1);
    uf.unite(1, 2);
    uf.unite(2, 3);

    uf.unite(10, 11);
    uf.unite(11, 12);

    check(uf.connected(0, 3),
          "0 and 3 should be connected");

    check(uf.connected(10, 12),
          "10 and 12 should be connected");

    check(!uf.connected(0, 10),
          "0 and 10 should not be connected");

    check(uf.size_of(0) == 4,
          "First set should have size 4");

    check(uf.size_of(12) == 3,
          "Second set should have size 3");

    uf.unite(3, 12);

    check(uf.connected(0, 10),
          "Sets should now be connected");

    check(uf.size_of(0) == 7,
          "Merged set should have size 7");

    check(uf.size_of(12) == 7,
          "Merged set should have size 7");
}

void test_repeated_union() {
    UnionFind uf(10);

    uf.unite(2, 3);
    uf.unite(2, 3);
    uf.unite(3, 2);
    uf.unite(2, 2);

    check(uf.connected(2, 3),
          "2 and 3 should be connected");

    check(uf.size_of(2) == 2,
          "Repeated union must not change set size");
}

void test_large_correctness() {
    constexpr std::size_t N = 100'000;

    UnionFind uf(N);

    // Make pairs.
    for (std::size_t i = 0; i + 1 < N; i += 2) {
        uf.unite(i, i + 1);
    }

    for (std::size_t i = 0; i + 1 < N; i += 2) {
        check(uf.connected(i, i + 1),
              "Pair union failed");
    }

    // Merge everything together.
    for (std::size_t step = 2; step < N; step *= 2) {
        for (std::size_t i = 0;
             i + step < N;
             i += 2 * step) {

            uf.unite(i, i + step);
        }
    }

    const auto root = uf.find(0);

    for (std::size_t i = 0; i < N; ++i) {
        check(uf.find(i) == root,
              "Large union failed");
    }

    check(uf.size_of(0) == N,
          "Large set size incorrect");
}

void run_correctness_tests() {
    std::cout << "=== UnionFind correctness tests ===\n";

    test_initial_state();
    test_basic_union();
    test_multiple_sets();
    test_repeated_union();
    test_large_correctness();

    std::cout << "All UnionFind correctness tests passed!\n\n";
}

void benchmark() {
    constexpr std::size_t N = 10'000'000;
    constexpr std::size_t QUERIES = 20'000'000;

    std::cout << "=== UnionFind benchmark ===\n";
    std::cout << "Elements: " << N << '\n';
    std::cout << "Queries : " << QUERIES << "\n\n";

    // --------------------------------------------------
    // Construction
    // --------------------------------------------------

    auto start = Clock::now();

    UnionFind uf(N);

    auto end = Clock::now();

    double construction_time =
        std::chrono::duration<double>(end - start).count();

    std::cout
        << "Construction:             "
        << construction_time
        << " s\n";

    // --------------------------------------------------
    // Union
    //
    // Deliberately merge equal-sized sets:
    //
    // step = 1:
    // (0,1), (2,3), ...
    //
    // step = 2:
    // (0,2), (4,6), ...
    //
    // step = 4:
    // (0,4), ...
    //
    // This creates a reasonably deep weighted tree before
    // path compression flattens it.
    // --------------------------------------------------

    start = Clock::now();

    for (std::size_t step = 1; step < N; step *= 2) {
        for (std::size_t i = 0;
             i + step < N;
             i += 2 * step) {

            uf.unite(i, i + step);
        }
    }

    end = Clock::now();

    double union_time =
        std::chrono::duration<double>(end - start).count();

    check(uf.size_of(0) == N,
          "Benchmark union produced incorrect size");

    check(uf.connected(0, N - 1),
          "Benchmark union failed to connect all elements");

    std::cout
        << "Union all elements:       "
        << union_time
        << " s\n";

    // --------------------------------------------------
    // First find pass
    //
    // This pass performs path compression.
    // --------------------------------------------------

    std::size_t checksum1 = 0;

    start = Clock::now();

    for (std::size_t i = 0; i < N; ++i) {
        checksum1 += uf.find(i);
    }

    end = Clock::now();

    double first_find_time =
        std::chrono::duration<double>(end - start).count();

    std::cout
        << "First 10M find pass:      "
        << first_find_time
        << " s\n";

    // --------------------------------------------------
    // Second find pass
    //
    // Most paths should now be compressed.
    // --------------------------------------------------

    std::size_t checksum2 = 0;

    start = Clock::now();

    for (std::size_t i = 0; i < N; ++i) {
        checksum2 += uf.find(i);
    }

    end = Clock::now();

    double second_find_time =
        std::chrono::duration<double>(end - start).count();

    std::cout
        << "Second 10M find pass:     "
        << second_find_time
        << " s\n";

    check(checksum1 == checksum2,
          "Find checksum mismatch");

    // --------------------------------------------------
    // connected()
    //
    // Deterministic pseudo-random-ish accesses so the CPU
    // does not merely walk the array sequentially.
    // --------------------------------------------------

    std::size_t connected_count = 0;

    start = Clock::now();

    for (std::size_t i = 0; i < QUERIES; ++i) {
        std::size_t a =
            (i * 7'919 + 12'345) % N;

        std::size_t b =
            (i * 104'729 + 97'531) % N;

        if (uf.connected(a, b)) {
            ++connected_count;
        }
    }

    end = Clock::now();

    double query_time =
        std::chrono::duration<double>(end - start).count();

    check(connected_count == QUERIES,
          "connected() benchmark returned false unexpectedly");

    double queries_per_second =
        static_cast<double>(QUERIES) / query_time;

    double nanoseconds_per_query =
        query_time * 1'000'000'000.0 /
        static_cast<double>(QUERIES);

    std::cout
        << "20M connected() queries:  "
        << query_time
        << " s\n";

    std::cout
        << std::fixed
        << std::setprecision(2);

    std::cout
        << "Throughput:               "
        << queries_per_second / 1'000'000.0
        << " million queries/sec\n";

    std::cout
        << "Average:                  "
        << nanoseconds_per_query
        << " ns/query\n";

    std::cout
        << "Checksum:                 "
        << checksum2
        << '\n';

    std::cout
        << "Connected count:          "
        << connected_count
        << "\n\n";

    std::cout
        << "Huge UnionFind benchmark passed!\n";
}

int main() {
    try {
        run_correctness_tests();
        benchmark();
    }
    catch (const std::exception& e) {
        std::cerr
            << "\nTEST FAILED: "
            << e.what()
            << '\n';

        return 1;
    }

    return 0;
}