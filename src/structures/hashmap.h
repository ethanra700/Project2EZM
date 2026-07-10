#pragma once

#include "../data/GymRecord.h"
#include <cstddef>
#include <utility>
#include <vector>

class HashMap {
public:
    struct SlotResult {
        int hour = -1; double avg_headcount = 0.0;
        bool found = false;
    };

    HashMap();

    ~HashMap();

    HashMap(const HashMap&) = delete;

    HashMap(HashMap&&) = delete;

    HashMap& operator=(const HashMap&) = delete;

    HashMap& operator=(HashMap&&) = delete;

    std::size_t size() const;
    void insert(const GymRecord& rec);
    SlotResult leastCongestedSlot(int day_of_week, int start_hour, int end_hour) const;

private:
    using SlotKey = std::pair<int, int>;
    using Headcounts = std::vector<int>;

    struct Entry {
        SlotKey key;
        Headcounts samples;
        Entry* next = nullptr;
        explicit Entry(SlotKey k) : key(k) {}
    };

    using Table = std::vector<Entry*>;

    static constexpr double kLoadCeiling = 3.0 / 4.0;

    std::size_t bucketFor(const SlotKey& key) const;
    Entry* locate(const SlotKey& key) const;
    void grow();

    Table table_;
    std::size_t entryCount_ = 0;
};
