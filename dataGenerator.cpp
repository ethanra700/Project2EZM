#include <iostream>
#include <fstream>
#include <random>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <string>
#include <cmath>
#include <algorithm>

struct GymData {
    int numPeople;
    std::string date;
    long timestamp;
    int dayOfWeek;
    int isWeekend;
    int isHoliday;
    double temperature;
    int isStartSemester;
    int isDuringSemester;
    int month;
    int hour;
};

// Helper function to calculate seconds since midnight
long getSecondsSinceMidnight(const std::tm& timeStruct) {
    return (timeStruct.tm_hour * 3600) + (timeStruct.tm_min * 60) + timeStruct.tm_sec;
}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::normal_distribution<> tempDist(58.56, 6.32);
    //Hourly mean distribution of people
    const double hourMean[24] = {14.72, 2.40, 0.18, 0.15, 0.28, 1.05, 8.76, 15.39, 24.05, 30.19, 33.73, 36.22, 36.11, 34.57, 33.93, 37.20, 42.96, 47.64, 48.30, 44.74, 41.67, 42.92, 40.46, 26.15};
    const double hourStd[24] = {15.35, 5.30, 0.44, 0.41, 0.55, 1.32, 8.15, 12.50, 13.66, 14.60, 16.11, 15.88, 15.00, 14.74, 14.95, 16.25, 19.35, 22.28, 21.19, 21.85, 21.91, 23.21, 22.51, 23.25};
    //artifical holiday distribution since its not accurately measured in dataset.
    std::uniform_int_distribution<> holidayDist(1, 100);
    //directly appending previous data text document
    std::string filename = "data.csv";
    std::ofstream outFile(filename, std::ios::app);

    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open file for writing." << std::endl;
        return 1;
    }

    //tm works weirdly and counts years after 1900, this directly continues off the previous dataset.
    std::tm timeStruct = {};
    timeStruct.tm_year = 2017 - 1900;
    timeStruct.tm_mon = 2;
    timeStruct.tm_mday = 18;
    timeStruct.tm_hour = 19;
    timeStruct.tm_min = 22;
    timeStruct.tm_sec = 51;
    timeStruct.tm_isdst = -1;

    std::mktime(&timeStruct);


    const int ROWS_TO_GENERATE = 37816; //# of remaining data points needed
    const int TIME_INCREMENT_SECONDS = 600;
    for (int i = 0; i < ROWS_TO_GENERATE; ++i) {
        GymData data;

        timeStruct.tm_sec += TIME_INCREMENT_SECONDS;
        std::mktime(&timeStruct);

        std::ostringstream dateStream;
        dateStream << std::put_time(&timeStruct, "%Y-%m-%d %H:%M:%S") << "-07:00";
        data.date = dateStream.str();

        data.timestamp = getSecondsSinceMidnight(timeStruct);

        // 0=Monday, 6=Sunday
        data.dayOfWeek = (timeStruct.tm_wday + 6) % 7;
        data.isWeekend = (data.dayOfWeek == 5 || data.dayOfWeek == 6) ? 1 : 0;
        data.month = timeStruct.tm_mon + 1;
        data.hour = timeStruct.tm_hour;

        std::normal_distribution<> peopleDist(hourMean[data.hour], hourStd[data.hour]);
        int generatedPeople = std::round(peopleDist(gen));

        data.numPeople = std::max(0, generatedPeople); 
        data.temperature = tempDist(gen);
        data.isHoliday = (holidayDist(gen) == 1) ? 1 : 0;
        data.isStartSemester = (data.month == 8 || data.month == 1) ? 1 : 0;
        data.isDuringSemester = (data.month >= 9 && data.month <= 11) || (data.month >= 2 && data.month <= 5) ? 1 : 0;


        outFile << data.numPeople << "," << data.date << "," << data.timestamp << "," << data.dayOfWeek << "," << data.isWeekend << "," << data.isHoliday << "," << std::fixed << std::setprecision(2) << data.temperature << "," << data.isStartSemester << "," << data.isDuringSemester << "," << data.month << "," << data.hour << "\n";
    }

    outFile.close();


    return 0;
}
