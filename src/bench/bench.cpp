#include "bench.h"
#include "loader.h"
#include "GymRecord.h"
#include "hashmap.h"
#include "heap.h"
#include <chrono>
#include <iostream>
#include <vector>

using Clock = std::chrono::steady_clock;

namespace {

double msSince(Clock::time_point start) {
    return std::chrono::duration<double, std::milli>(Clock::now() - start).count();
}

struct QuerySpec {
    int day;
    int start_hour;
    int end_hour;
};

void runSize(const std::vector<GymRecord>& all, std::size_t n) {
    std::vector<GymRecord> subset(all.begin(), all.begin() + n);

    HashMap map;
    auto t0 = Clock::now();
    for (auto& rec : subset) map.insert(rec);
    double mapBuildMs = msSince(t0);

    Heap heap;
    t0 = Clock::now();
    for (auto& rec : subset) heap.insert(rec);
    double heapBuildMs = msSince(t0);

    std::vector<QuerySpec> queries = {
        {0, 0, 23}, {1, 6, 9}, {2, 12, 14}, {3, 0, 5}, {4, 17, 23},
    };

    const int reps = 2000;
    const double totalQueries = static_cast<double>(reps) * queries.size();

    t0 = Clock::now();
    for (int r = 0; r < reps; ++r) {
        for (auto& q : queries) map.leastCongestedSlot(q.day, q.start_hour, q.end_hour);
    }
    double mapQueryMs = msSince(t0);

    t0 = Clock::now();
    for (int r = 0; r < reps; ++r) {
        for (auto& q : queries) heap.leastCongestedSlot(q.day, q.start_hour, q.end_hour);
    }
    double heapQueryMs = msSince(t0);

    std::cout << n << ","
              << mapBuildMs << ","
              << heapBuildMs << ","
              << (mapQueryMs * 1000.0 / totalQueries) << ","
              << (heapQueryMs * 1000.0 / totalQueries) << "\n";
}

}

void runBenchmarks() {
    auto all = loadRecords("data.csv");
    if (all.empty()) {
        std::cout << "no data loaded from data.csv\n";
        return;
    }

    std::cout << "rows,hashmap_build_ms,heap_build_ms,hashmap_query_us,heap_query_us\n";

    std::vector<std::size_t> sizes = {10000, 25000, 50000, 100000};
    for (auto n : sizes) {
        if (n > all.size()) n = all.size();
        runSize(all, n);
    }
}
