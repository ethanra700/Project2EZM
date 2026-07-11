#include "loader.h"
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <cmath>

using namespace std;

namespace {

    string strip(const string& text) {
        auto head = size_t{0};
        
        auto tail =text.size();
        
        for (; tail > head; ) {
            if (isspace(static_cast<unsigned char>(text[head]))) {
                head =head + 1;
            } else {
                break;
            }
        }
        for (; tail >head; ) {
            if (isspace(static_cast<unsigned char>(text[tail - 1]))) {
                tail =tail - 1;
            } else {
                break;
            }
        }
        return text.substr(head, tail - head);
    }

    template <typename T>
    bool readNumber(const string& text, T& out) {
        auto cellStream = istringstream(text);
        cellStream >> out;
        return cellStream.fail() == false;
    }

    vector<string> splitRow(const string& row) {
        auto columns = vector<string>{};
        columns.reserve(11);
        auto rowStream= istringstream(row);
        auto token = string{};
        for (; getline(rowStream, token, ','); ) {
            columns.push_back(strip(token));
        }
        return columns;
    }

    bool buildRecord(const string& row, GymRecord& entry) {
        if (strip(row).empty() == true) return false;
        auto columns = splitRow(row);
        if (11 > columns.size()) return false;

        auto headcount = 0;
        auto timestamp = 0L;
        auto dayOfWeek = 0;
        auto holidayFlag = 0;
        auto tempRaw = 0.0;
        auto hourValue = 0;

        auto ok = readNumber(columns[0], headcount);
        if (ok == true) ok = readNumber(columns[2], timestamp);
        if (ok == true) ok = readNumber(columns[3], dayOfWeek);
        if (ok == true) ok = readNumber(columns[5], holidayFlag);
        if (ok == true) ok = readNumber(columns[6], tempRaw);
        if (ok == true) ok = readNumber(columns[10], hourValue);
        if (ok == false) return false;

        entry.headcount = headcount;
        entry.timestamp = timestamp;
        entry.day_of_week = dayOfWeek;
        entry.is_holiday = (1 == holidayFlag);
        entry.temperature = static_cast<int>(lround(tempRaw));
        entry.hour = hourValue;
        return true;
    }
}


vector<GymRecord> loadRecords(const string& filePath) {
    auto entries = vector<GymRecord>{};
    auto input = ifstream(filePath);
    if (input.is_open() == false) return entries;

    entries.reserve(100000);
    auto headerPending = true;
    auto row = string{};

    for (; getline(input, row); ) {
        if (headerPending == true) {
            headerPending = false;
        } else {
            auto entry = GymRecord{};
            if (buildRecord(row, entry) == true) entries.push_back(entry);
        }
    }
    return entries;
}


