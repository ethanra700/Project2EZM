#include "MainWindow.h"
#include "heap.h"

#include <QDate>
#include <QLabel>
#include <QLineEdit>
#include <QRegularExpression>
#include <QTime>
#include <QVBoxLayout>
#include <QWidget>
#include <chrono>

namespace {
const QStringList kDayNames = {"Sunday", "Monday", "Tuesday", "Wednesday",
                                "Thursday", "Friday", "Saturday"};
const QStringList kMenuItems = {
    "1. Find least congested time today",
    "2. Check congestion for a date and time",
    "3. Compare congestion across the week",
    "4. Run benchmark (Heap vs Hash Map)"};
} // namespace

MainWindow::MainWindow(std::vector<GymRecord> records, QWidget* parent)
    : QMainWindow(parent), records_(std::move(records)) {
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

    auto* central = new QWidget(this);
    central->setObjectName("central");
    auto* outer = new QVBoxLayout(central);
    outer->setContentsMargins(32, 32, 32, 32);
    outer->setSpacing(22);
    outer->addStretch(1);

    auto* card = new QWidget(central);
    card->setObjectName("card");
    card->setFixedWidth(572);
    auto* cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(44, 32, 44, 32);
    cardLayout->setSpacing(0);

    auto* titleLabel = new QLabel("Wait Less Lift More", card);
    titleLabel->setObjectName("titleLabel");
    titleLabel->setAlignment(Qt::AlignHCenter);
    cardLayout->addWidget(titleLabel);
    cardLayout->addSpacing(31);

    auto* separator = new QWidget(card);
    separator->setObjectName("separator");
    separator->setFixedHeight(4);
    cardLayout->addWidget(separator);
    cardLayout->addSpacing(15);

    for (const QString& item : kMenuItems) {
        auto* menuLabel = new QLabel(item, card);
        menuLabel->setObjectName("menuLabel");
        menuLabel->setFixedHeight(29);
        cardLayout->addWidget(menuLabel);
    }
    cardLayout->addSpacing(60);

    promptLabel_ = new QLabel("Enter choice", card);
    promptLabel_->setObjectName("promptLabel");
    cardLayout->addWidget(promptLabel_);
    cardLayout->addSpacing(8);

    choiceEdit_ = new QLineEdit(card);
    choiceEdit_->setObjectName("choiceEdit");
    choiceEdit_->setFixedHeight(58);
    connect(choiceEdit_, &QLineEdit::returnPressed, this, &MainWindow::onEnter);
    cardLayout->addWidget(choiceEdit_);

    outer->addWidget(card, 0, Qt::AlignHCenter);

    infoBox_ = new QWidget(central);
    infoBox_->setObjectName("infoBox");
    infoBox_->setFixedWidth(572);
    infoBox_->setVisible(false);
    auto* infoLayout = new QVBoxLayout(infoBox_);
    infoLayout->setContentsMargins(20, 20, 20, 20);
    infoLayout->setSpacing(9);

    infoHeadingLabel_ = new QLabel(infoBox_);
    infoHeadingLabel_->setObjectName("infoHeading");
    infoHeadingLabel_->setWordWrap(true);
    infoLayout->addWidget(infoHeadingLabel_);

    infoMessageLabel_ = new QLabel(infoBox_);
    infoMessageLabel_->setObjectName("infoMessage");
    infoMessageLabel_->setWordWrap(true);
    infoLayout->addWidget(infoMessageLabel_);

    infoDetailsLayout_ = new QVBoxLayout();
    infoDetailsLayout_->setSpacing(4);
    infoLayout->addLayout(infoDetailsLayout_);

    outer->addWidget(infoBox_, 0, Qt::AlignHCenter);
    outer->addStretch(2);

    setCentralWidget(central);
    setWindowTitle("Wait Less Lift More");
    resize(636, 900);

    setStyleSheet(R"(
        QWidget#central { background-color: #001540; }
        QWidget#card { background-color: #001C57; border-radius: 24px; }
        QWidget#infoBox { background-color: #001C57; border: 1px solid #00316E; border-radius: 21px; }
        QWidget#separator { background-color: #001540; border-radius: 2px; }
        QLabel#titleLabel { color: #f1f1ef; font-family: Arial; font-size: 17px; font-weight: bold; }
        QLabel#menuLabel { color: #dededb; font-family: Arial; font-size: 14px; }
        QLabel#promptLabel { color: #d0d1cd; font-family: Arial; font-size: 20px; }
        QLabel#infoMessage { color: #ededea; font-family: Arial; font-size: 14px; }
        QLabel#detailLabel { color: #c6c7c2; font-family: Arial; font-size: 13px; }
        QLineEdit#choiceEdit {
            background-color: #001540;
            color: #f0f0ed;
            border: 2px solid #00316E;
            border-radius: 21px;
            font-family: Arial;
            font-size: 20px;
            padding-left: 16px;
            padding-right: 16px;
        }
    )");

    choiceEdit_->setFocus();
}

void MainWindow::updatePrompt() {
    if (step_ == Step::Choice) {
        promptLabel_->setText("Enter choice");
    } else if (step_ == Step::Date) {
        promptLabel_->setText("Enter date");
    } else {
        promptLabel_->setText("Enter time");
    }
}

void MainWindow::showInfo(const ResultInfo& info, bool isError) {
    infoBox_->setVisible(true);
    infoHeadingLabel_->setText(info.heading);
    infoHeadingLabel_->setStyleSheet(QString("color: %1; font-family: Arial; font-size: 17px; font-weight: bold;")
        .arg(isError ? "#e74c3c" : "#78e08f"));
    infoMessageLabel_->setText(info.message);

    QLayoutItem* item;
    while ((item = infoDetailsLayout_->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    for (const QString& line : info.details) {
        auto* detailLabel = new QLabel("- " + line, infoBox_);
        detailLabel->setObjectName("detailLabel");
        detailLabel->setWordWrap(true);
        infoDetailsLayout_->addWidget(detailLabel);
    }
}

void MainWindow::badInput(const QString& msg) {
    showInfo({"Try again", msg, {"Press Enter after correcting the input."}}, true);
}

void MainWindow::choose(const QString& num) {
    static const QRegularExpression menuChoice("^[1-4]$");
    if (!menuChoice.match(num).hasMatch()) {
        badInput("Enter a menu number from 1 to 4.");
        return;
    }
    if (num == "1") {
        showInfo(leastCongestedTimeResult(), false);
    } else if (num == "2") {
        step_ = Step::Date;
        showInfo(datePromptResult(), false);
    } else if (num == "3") {
        showInfo(weeklyComparisonResult(), false);
    } else {
        showInfo(benchmarkResult(), false);
    }
}

void MainWindow::onEnter() {
    QString val = choiceEdit_->text().trimmed();
    if (val.isEmpty()) {
        return;
    }

    if (step_ == Step::Choice) {
        choose(val);
    } else if (step_ == Step::Date) {
        if (!dateOk(val)) {
            badInput("Use a real month and day in MM/DD format. Do not include a year.");
            choiceEdit_->clear();
            return;
        }
        date_ = fixDate(val);
        step_ = Step::Time;
        showInfo(timePromptResult(date_), false);
    } else {
        if (!timeOk(val)) {
            badInput("Use a time such as 8:30 AM or 4:15 PM.");
            choiceEdit_->clear();
            return;
        }
        showInfo(congestionForResult(date_, fixTime(val)), false);
        step_ = Step::Choice;
        date_.clear();
    }
    choiceEdit_->clear();
    updatePrompt();
}

bool MainWindow::hasData() const {
    return validRecordCount_ > 0;
}

MainWindow::ResultInfo MainWindow::unavailableDataResult() const {
    return {"Gym data unavailable",
            "No readable records were loaded, so a live congestion result cannot be calculated.",
            {}};
}

MainWindow::ResultInfo MainWindow::datePromptResult() const {
    return {"Check congestion",
            QString("Enter a month and day to compare its weekday pattern in the %1 calendar.")
                .arg(QDate::currentDate().year()),
            {"Format: MM/DD", "Example: 07/10"}};
}

MainWindow::ResultInfo MainWindow::timePromptResult(const QString& date) const {
    return {"Check congestion — " + date,
            "Enter the time you want to check.",
            {"Format: Hour:Min AM/PM", "Example: 5:30 PM"}};
}

MainWindow::ResultInfo MainWindow::leastCongestedTimeResult() const {
    if (!hasData()) {
        return unavailableDataResult();
    }
    const QDate today = QDate::currentDate();
    const int day = today.dayOfWeek() % 7;
    const auto slot = slotIndex_.leastCongestedSlot(day, 0, 23);
    if (!slot.found) {
        return {"No data for " + kDayNames[day],
                "The loaded CSV has no usable records for today.",
                {"Loaded records: " + QString::number(validRecordCount_)}};
    }
    const int displayHour = slot.hour % 12 == 0 ? 12 : slot.hour % 12;
    const QString timeLabel = QString("%1:00 %2").arg(displayHour).arg(slot.hour < 12 ? "AM" : "PM");
    const QString occupancyLabel = QString::number(slot.avg_headcount, 'f', 1) + " average visitors";
    return {"Least congested time today",
            timeLabel + " is the quietest recorded hour for " + kDayNames[day] + ".",
            {"Expected occupancy: " + occupancyLabel,
             "Based on " + QString::number(validRecordCount_) + " loaded records."}};
}

MainWindow::ResultInfo MainWindow::congestionForResult(const QString& date, const QString& time) const {
    if (!hasData()) {
        return unavailableDataResult();
    }
    const int year = QDate::currentDate().year();
    const QDate requestedDate = QDate::fromString(date + "/" + QString::number(year), "MM/dd/yyyy");
    const QTime requestedTime = QTime::fromString(time, "h:mm AP");
    if (!requestedDate.isValid() || !requestedTime.isValid()) {
        return {"Invalid date or time", "Use a valid MM/DD date and Hour:Min AM/PM time.", {}};
    }
    const int day = requestedDate.dayOfWeek() % 7;
    const auto slot = slotIndex_.leastCongestedSlot(day, requestedTime.hour(), requestedTime.hour());
    const int displayHour = requestedTime.hour() % 12 == 0 ? 12 : requestedTime.hour() % 12;
    const QString timeLabel = QString("%1:00 %2").arg(displayHour).arg(requestedTime.hour() < 12 ? "AM" : "PM");
    if (!slot.found) {
        return {"No matching data",
                "The CSV has no records for " + kDayNames[day] + " at " + timeLabel + ".",
                {"Try another time or add data for this hour."}};
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
    return {"Congestion for " + date + " at " + timeLabel,
            congestion,
            {"Expected occupancy: " + occupancyLabel,
             "Overall dataset average: " + overallAverageLabel,
             "Matching weekday: " + kDayNames[day]}};
}

MainWindow::ResultInfo MainWindow::weeklyComparisonResult() const {
    if (!hasData()) {
        return unavailableDataResult();
    }
    std::vector<long long> totals(7, 0);
    std::vector<int> counts(7, 0);
    for (const GymRecord& record : records_) {
        if (record.day_of_week >= 0 && record.day_of_week <= 6) {
            totals[record.day_of_week] += record.headcount;
            ++counts[record.day_of_week];
        }
    }
    int bestDay = -1;
    int busiestDay = -1;
    for (int day = 0; day < 7; ++day) {
        if (counts[day] == 0) {
            continue;
        }
        if (bestDay == -1 || totals[day] * counts[bestDay] < totals[bestDay] * counts[day]) {
            bestDay = day;
        }
        if (busiestDay == -1 || totals[day] * counts[busiestDay] > totals[busiestDay] * counts[day]) {
            busiestDay = day;
        }
    }
    if (bestDay == -1) {
        return unavailableDataResult();
    }
    auto avg = [&](int i) { return double(totals[i]) / counts[i]; };
    const QString quietestOccupancy = QString::number(avg(bestDay), 'f', 1) + " average visitors";
    const QString busiestOccupancy = QString::number(avg(busiestDay), 'f', 1) + " average visitors";
    const QString overallOccupancy = QString::number(overallAverage_, 'f', 1) + " average visitors";
    return {"Weekly congestion comparison",
            kDayNames[bestDay] + " has the lowest average occupancy in the loaded data.",
            {"Quietest: " + kDayNames[bestDay] + " — " + quietestOccupancy,
             "Busiest: " + kDayNames[busiestDay] + " — " + busiestOccupancy,
             "Overall: " + overallOccupancy}};
}

MainWindow::ResultInfo MainWindow::benchmarkResult() const {
    if (!hasData()) {
        return unavailableDataResult();
    }
    using Clock = std::chrono::steady_clock;
    constexpr int repetitions = 250;
    double sink = 0.0;

    HashMap map;
    auto started = Clock::now();
    for (const GymRecord& record : records_) map.insert(record);
    const double mapBuildMs = std::chrono::duration<double, std::milli>(Clock::now() - started).count();

    Heap heap;
    started = Clock::now();
    for (const GymRecord& record : records_) heap.insert(record);
    const double heapBuildMs = std::chrono::duration<double, std::milli>(Clock::now() - started).count();

    started = Clock::now();
    for (int iteration = 0; iteration < repetitions; ++iteration) {
        for (int day = 0; day < 7; ++day) {
            const auto slot = map.leastCongestedSlot(day, 9, 10);
            sink += slot.avg_headcount;
        }
    }
    const double mapQueryUs = std::chrono::duration<double, std::micro>(Clock::now() - started).count() / (repetitions * 7);

    started = Clock::now();
    for (int iteration = 0; iteration < repetitions; ++iteration) {
        for (int day = 0; day < 7; ++day) {
            const auto slot = heap.leastCongestedSlot(day, 9, 10);
            sink += slot.avg_headcount;
        }
    }
    const double heapQueryUs = std::chrono::duration<double, std::micro>(Clock::now() - started).count() / (repetitions * 7);

    (void)sink;
    return {"Benchmark: Heap vs. Hash Map",
            "Measured with " + QString::number(validRecordCount_) + " valid loaded records and " +
                QString::number(repetitions * 7) + " least-congested-time queries per structure.",
            {"Hash map build: " + QString::number(mapBuildMs, 'f', 2) + " ms",
             "Heap build: " + QString::number(heapBuildMs, 'f', 2) + " ms",
             "Hash map average query: " + QString::number(mapQueryUs, 'f', 2) + " µs",
             "Heap average query: " + QString::number(heapQueryUs, 'f', 2) + " µs"}};
}

bool MainWindow::dateOk(const QString& val) {
    static const QRegularExpression re("^(0?[1-9]|1[0-2])/(0?[1-9]|[12][0-9]|3[01])$");
    const auto match = re.match(val);
    if (!match.hasMatch()) {
        return false;
    }
    const int month = match.captured(1).toInt();
    const int day = match.captured(2).toInt();
    return day <= QDate(2000, month, 1).daysInMonth();
}

bool MainWindow::timeOk(const QString& val) {
    static const QRegularExpression re("^(0?[1-9]|1[0-2]):[0-5][0-9]\\s?(AM|PM)$",
                                        QRegularExpression::CaseInsensitiveOption);
    return re.match(val).hasMatch();
}

QString MainWindow::fixDate(const QString& val) {
    const QStringList parts = val.split("/");
    const QString first = parts[0].length() == 1 ? "0" + parts[0] : parts[0];
    const QString second = parts[1].length() == 1 ? "0" + parts[1] : parts[1];
    return first + "/" + second;
}

QString MainWindow::fixTime(const QString& val) {
    static const QRegularExpression re("^(0?[1-9]|1[0-2]):([0-5][0-9])\\s?(AM|PM)$",
                                        QRegularExpression::CaseInsensitiveOption);
    const auto match = re.match(val);
    return QString("%1:%2 %3").arg(match.captured(1).toInt()).arg(match.captured(2)).arg(match.captured(3).toUpper());
}
