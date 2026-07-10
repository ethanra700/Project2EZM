#pragma once

#include "../data/GymRecord.h"
#include <array>
#include <vector>

class Heap {
public:
    struct SlotResult {
        int hour = -1;
        double avg_headcount = 0.0;
        bool found = false;
    };

    Heap();

    void insert(const GymRecord& rec);
    SlotResult leastCongestedSlot(int day_of_week, int start_hour, int end_hour);

private:
    struct Node {
        int hour = -1;
        double avg = 0.0;
        long long count = 0;
    };

    static constexpr int kDays = 7;
    static constexpr int kHours = 24;

    std::array<std::vector<Node>, kDays> heaps_;
    std::array<std::array<int, kHours>, kDays> pos_;

    static bool less(const Node& a, const Node& b);
    void swapNodes(int day, int i, int j);
    void siftUp(int day, int idx);
    void siftDown(int day, int idx);
    Node popMin(int day);
    void pushBack(int day, Node node);
};
