#include "BackendService.h"
#include "src/data/loader.h"
#include "src/structures/heap.h"
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QTime>
#include <QtGlobal>
#include <vector>
#include <chrono>
//Load data
QString locateDataFile() {
    QFileInfo file("../data.csv");
    return (file.isFile() && file.isReadable()) ? file.absoluteFilePath() : QString();
}
BackendService::BackendService(QObject* parent) : QObject(parent) {
    dataPath_ = locateDataFile();
    if (dataPath_.isEmpty()) {
        return;
    }
    records_ = loadRecords(dataPath_.toStdString());
    long long totalHeadcount = 0;
    for (const GymRecord& record : records_) {
        if (record.day_of_week < 0 || record.day_of_week > 6 || record.hour < 0 || record.hour > 23) {
            continue;
        }
        slotIndex_.insert(record);
        totalHeadcount += record.headcount;
        ++validRecordCount_;
    }
    if (validRecordCount_ > 0) {
        overallAverage_ = static_cast<double>(totalHeadcount) / validRecordCount_;
    }
}
BackendService* BackendService::create(QQmlEngine* engine, QJSEngine* js) {
    Q_UNUSED(engine)
    Q_UNUSED(js)
    return new BackendService;
}
bool BackendService::hasData() const {
    return validRecordCount_ > 0;
}
QVariantMap BackendService::unavailableDataResult() const {
    return {{"heading", "Gym data unavailable"},{"message", "No readable records were loaded, so a live congestion result cannot be calculated."},{"details", QVariant::fromValue(QStringList{})},
    };
}
QVariantMap BackendService::datePrompt() const {
    return {{"heading", "Check congestion"},{"message", "Enter a month and day to compare its weekday pattern in the " + QString::number(QDate::currentDate().year()) + " calendar."},{"details", QVariant::fromValue(QStringList{"Format: MM/DD", "Example: 07/10"})},};
}
QVariantMap BackendService::timePrompt(const QString& date) const {
    return {{"heading", "Check congestion — " + date},{"message", "Enter the time you want to check."},{"details", QVariant::fromValue(QStringList{"Format: Hour:Min AM/PM", "Example: 5:30 PM"})},
    };
}
QVariantMap BackendService::leastCongestedTime() const {
    if (!hasData()) {
        return unavailableDataResult();
    }
    static const std::vector<QString> kDayNames = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    const QDate today = QDate::currentDate();
    const int day = today.dayOfWeek() % 7;
    const auto slot = slotIndex_.leastCongestedSlot(day, 0, 23);
    if (!slot.found) {
        return {{"heading", "No data for " + kDayNames[day]},{"message", "The loaded CSV has no usable records for today."},{"details", QVariant::fromValue(QStringList{"Loaded records: " + QString::number(validRecordCount_)})},};
    }
    const int displayHour = slot.hour % 12 == 0 ? 12 : slot.hour % 12;
    const QString timeLabel = QStringLiteral("%1:00 %2").arg(displayHour).arg(slot.hour < 12 ? "AM" : "PM");
    const QString occupancyLabel = QString::number(slot.avg_headcount, 'f', 1) + " average visitors";
    return {{"heading", "Least congested time today"},{"message", timeLabel + " is the quietest recorded hour for " + kDayNames[day] + "."},{"details", QVariant::fromValue(QStringList{"Expected occupancy: " + occupancyLabel,"Based on " + QString::number(validRecordCount_) + " loaded records.",})},};
}

QVariantMap BackendService::congestionFor(const QString& date, const QString& time) const {
    if (!hasData()) {
        return unavailableDataResult();
    }
    static const std::vector<QString> kDayNames = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    const int year = QDate::currentDate().year();
    const QDate requestedDate = QDate::fromString(date + "/" + QString::number(year), "MM/dd/yyyy");
    const QTime requestedTime = QTime::fromString(time, "h:mm AP");
    if (!requestedDate.isValid() || !requestedTime.isValid()) {
        return {{"heading", "Invalid date or time"},{"message", "Use a valid MM/DD date and Hour:Min AM/PM time."},{"details", QVariant::fromValue(QStringList{})},};
    }
    const int day = requestedDate.dayOfWeek() % 7;
    const auto slot = slotIndex_.slotFor(day, requestedTime.hour());
    const int displayHour = requestedTime.hour() % 12 == 0 ? 12 : requestedTime.hour() % 12;
    const QString timeLabel = QStringLiteral("%1:00 %2").arg(displayHour).arg(requestedTime.hour() < 12 ? "AM" : "PM");
    if (!slot.found) {
        return {{"heading", "No matching data"},{"message", "The CSV has no records for " + kDayNames[day] + " at " + timeLabel + "."},{"details", QVariant::fromValue(QStringList{"Try another time or add data for this hour."})},};
    }
    QString congestion;
    if (overallAverage_ <= 0.0 || slot.avg_headcount <= overallAverage_ * 0.75) {
        congestion = "Low congestion";
    } else if (slot.avg_headcount <= overallAverage_ * 1.25) {
        congestion = "Moderate congestion";
    } else {
        congestion = "High congestion";
    }
    const QString occupancyLabel = QString::number(slot.avg_headcount, 'f', 1) + " average visitors";
    const QString overallAverageLabel = QString::number(overallAverage_, 'f', 1) + " average visitors";
    return {{"heading", "Congestion for " + date + " at " + timeLabel},{"message", congestion},{"details", QVariant::fromValue(QStringList{"Expected occupancy: " + occupancyLabel,"Overall dataset average: " + overallAverageLabel,"Matching weekday: " + kDayNames[day],})},};
}

