#include "hashmap.h"
#include <algorithm>
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

HashMap::SlotResult HashMap::slotFor(int day_of_week, int hour) const {
    SlotResult result;
    if (day_of_week < 0 || day_of_week > 6 || hour < 0 || hour > 23) {
        return result;
    }

    const auto* node = locate(SlotKey{day_of_week, hour});
    if (node == nullptr || node->samples.empty()) {
        return result;
    }

    long long sum = 0;
    for (const int sample : node->samples) {
        sum += sample;
    }

    result.hour = hour;
    result.avg_headcount = static_cast<double>(sum) / node->samples.size();
    result.found = true;
    return result;
}

HashMap::SlotResult HashMap::leastCongestedSlot(int day_of_week, int start_hour,int end_hour) const {
    if (day_of_week < 0 || day_of_week > 6) {
        return {};
    }

    start_hour = std::max(0, start_hour);
    end_hour = std::min(23, end_hour);
    if (end_hour < start_hour) {
        swap(start_hour, end_hour);
    }

    SlotResult best;
    int hour = start_hour;
    while (end_hour >= hour) {
        const auto candidate = slotFor(day_of_week, hour);
        if (candidate.found && (!best.found || candidate.avg_headcount < best.avg_headcount)) {
            best = candidate;
        }
        hour = hour + 1;
    }
    return best;
}
