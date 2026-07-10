#include "hashmap.h"
#include <utility>

using namespace std;

namespace {
    constexpr size_t kSeedBuckets = 16;
}

HashMap::HashMap():table_(kSeedBuckets, nullptr) {}

HashMap::~HashMap() {
    auto destroyChain = [](Entry* head) {
        while (head != nullptr) {
            auto* nextEntry = head->next;
            delete head;
          
            head =nextEntry;
        }
    };

    size_t slot = 0;
    while (table_.size() > slot) {
        destroyChain(table_[slot]);
        slot = slot + 1;
    }
}

size_t HashMap::size() const {
    return entryCount_;
}

void HashMap::grow() {
    auto old = table_;
    table_.assign(old.size() * 2, nullptr);

    size_t slot = 0;
    while (old.size() >slot) {
        auto* head= old[slot];
        while (head != nullptr) {
            auto* nextEntry = head->next;
            auto idx = bucketFor(head->key);
            head->next = table_[idx];
            table_[idx] = head;
            head = nextEntry;
        }
      
        slot = slot + 1;
    }
}

size_t HashMap::bucketFor(const SlotKey& key) const {
    auto folded = static_cast<unsigned long long>(key.first) * 24ULL
                + static_cast<unsigned long long>(key.second);
    folded = folded * 2654435761ULL;
    return static_cast<size_t>(folded % table_.size());
}

HashMap::Entry* HashMap::locate(const SlotKey& key) const {
    auto* cur = table_[bucketFor(key)];
    while (cur != nullptr) {
        if (cur->key == key) {
            return cur;
        }
        cur = cur->next;
    }
    return nullptr;
}

void HashMap::insert(const GymRecord& rec) {
    SlotKey key{rec.day_of_week, rec.hour};

    if (kLoadCeiling * table_.size() < entryCount_ + 1) {
      
        grow();
      
    }

    auto* node = locate(key);
    if (node == nullptr) {
        auto idx = bucketFor(key); node = new Entry(key);
        node->next = table_[idx];
        table_[idx]= node;
        entryCount_ = entryCount_ + 1;
    }
    node->samples.push_back(rec.headcount);
}

HashMap::SlotResult HashMap::leastCongestedSlot(int day_of_week, int start_hour,int end_hour) const {
    if (end_hour < start_hour) {
        swap(start_hour, end_hour);
    }

    auto averageOf = [](const Headcounts& samples) -> double {
        long long sum = 0;
        size_t i = 0;
        while (samples.size() > i) {
            sum = sum + samples[i];
            i = i + 1;
        }
        return static_cast<double>(sum) / samples.size();
    };

    SlotResult best;
    int hour = start_hour;
    while (end_hour >= hour) {
        auto* node = locate(SlotKey{day_of_week, hour});
        if (node != nullptr) {
            if (!node->samples.empty()) {
                auto avg = averageOf(node->samples);

                bool improves = false;
                if (!best.found) {
                    improves= true;
                } else if (best.avg_headcount > avg) {
                    improves = true;}

                if (improves) { best.hour = hour;
                    best.avg_headcount = avg;
                    best.found = true;
                }
            }
        }
        hour = hour + 1;
    }
    return best;
}
