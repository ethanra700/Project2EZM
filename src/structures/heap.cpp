#include "heap.h"
#include <utility>

Heap::Heap() {
    for (auto& row : pos_) row.fill(-1);
}

bool Heap::less(const Node& a, const Node& b) {
    return a.avg < b.avg;
}

void Heap::swapNodes(int day, int i, int j) {
    auto& h = heaps_[day];
    std::swap(h[i], h[j]);
    pos_[day][h[i].hour] = i;
    pos_[day][h[j].hour] = j;
}

void Heap::siftUp(int day, int idx) {
    auto& h = heaps_[day];
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (!less(h[idx], h[parent])) break;
        swapNodes(day, idx, parent);
        idx = parent;
    }
}

void Heap::siftDown(int day, int idx) {
    auto& h = heaps_[day];
    int n = static_cast<int>(h.size());
    while (true) {
        int left = idx * 2 + 1;
        int right = idx * 2 + 2;
        int smallest = idx;
        if (left < n && less(h[left], h[smallest])) smallest = left;
        if (right < n && less(h[right], h[smallest])) smallest = right;
        if (smallest == idx) break;
        swapNodes(day, idx, smallest);
        idx = smallest;
    }
}

void Heap::insert(const GymRecord& rec) {
    int day = rec.day_of_week;
    int hour = rec.hour;
    int idx = pos_[day][hour];

    if (idx == -1) {
        heaps_[day].push_back(Node{hour, static_cast<double>(rec.headcount), 1});
        idx = static_cast<int>(heaps_[day].size()) - 1;
        pos_[day][hour] = idx;
        siftUp(day, idx);
        return;
    }

    Node& node = heaps_[day][idx];
    node.count += 1;
    double oldAvg = node.avg;
    node.avg = oldAvg + (static_cast<double>(rec.headcount) - oldAvg) / static_cast<double>(node.count);

    if (node.avg < oldAvg) {
        siftUp(day, idx);
    } else if (node.avg > oldAvg) {
        siftDown(day, idx);
    }
}

Heap::Node Heap::popMin(int day) {
    auto& h = heaps_[day];
    Node root = h.front();
    pos_[day][root.hour] = -1;

    Node last = h.back();
    h.pop_back();
    if (!h.empty()) {
        h[0] = last;
        pos_[day][last.hour] = 0;
        siftDown(day, 0);
    }
    return root;
}

void Heap::pushBack(int day, Node node) {
    auto& h = heaps_[day];
    h.push_back(node);
    int idx = static_cast<int>(h.size()) - 1;
    pos_[day][node.hour] = idx;
    siftUp(day, idx);
}

Heap::SlotResult Heap::leastCongestedSlot(int day_of_week, int start_hour, int end_hour) {
    if (end_hour < start_hour) std::swap(start_hour, end_hour);

    SlotResult result;
    std::vector<Node> stash;

    while (!heaps_[day_of_week].empty()) {
        Node top = popMin(day_of_week);
        bool inRange = top.hour >= start_hour && top.hour <= end_hour;
        stash.push_back(top);
        if (inRange) {
            result.hour = top.hour;
            result.avg_headcount = top.avg;
            result.found = true;
            break;
        }
    }

    for (auto& node : stash) {
        pushBack(day_of_week, node);
    }

    return result;
}
