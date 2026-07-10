#include "MainWindow.h"
#include "hashmap.h"
#include "heap.h"

#include <QCheckBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>
#include <chrono>

MainWindow::MainWindow(std::vector<GymRecord> records, QWidget* parent)
    : QMainWindow(parent), records_(std::move(records)) {
    auto* central = new QWidget(this);
    auto* layout = new QVBoxLayout(central);
    auto* form = new QFormLayout();

    daySpin_ = new QSpinBox(central);
    daySpin_->setRange(0, 6);

    startHourSpin_ = new QSpinBox(central);
    startHourSpin_->setRange(0, 23);

    endHourSpin_ = new QSpinBox(central);
    endHourSpin_->setRange(0, 23);
    endHourSpin_->setValue(23);

    tempFilterCheck_ = new QCheckBox("Filter by temperature", central);

    tempMinSpin_ = new QSpinBox(central);
    tempMinSpin_->setRange(-50, 150);
    tempMinSpin_->setValue(0);

    tempMaxSpin_ = new QSpinBox(central);
    tempMaxSpin_->setRange(-50, 150);
    tempMaxSpin_->setValue(150);

    form->addRow("Day of week (0=Sun)", daySpin_);
    form->addRow("Start hour", startHourSpin_);
    form->addRow("End hour", endHourSpin_);
    form->addRow(tempFilterCheck_);

    auto* tempRow = new QHBoxLayout();
    tempRow->addWidget(new QLabel("Min", central));
    tempRow->addWidget(tempMinSpin_);
    tempRow->addWidget(new QLabel("Max", central));
    tempRow->addWidget(tempMaxSpin_);
    form->addRow(tempRow);

    layout->addLayout(form);

    auto* runButton = new QPushButton("Find least congested slot", central);
    connect(runButton, &QPushButton::clicked, this, &MainWindow::runQuery);
    layout->addWidget(runButton);

    hashResultLabel_ = new QLabel(central);
    heapResultLabel_ = new QLabel(central);
    layout->addWidget(hashResultLabel_);
    layout->addWidget(heapResultLabel_);

    setCentralWidget(central);
    setWindowTitle("Gym Congestion Predictor");
}

void MainWindow::runQuery() {
    int day = daySpin_->value();
    int startHour = startHourSpin_->value();
    int endHour = endHourSpin_->value();

    std::vector<GymRecord> subset;
    if (tempFilterCheck_->isChecked()) {
        int tmin = tempMinSpin_->value();
        int tmax = tempMaxSpin_->value();
        for (auto& rec : records_) {
            if (rec.temperature >= tmin && rec.temperature <= tmax) subset.push_back(rec);
        }
    } else {
        subset = records_;
    }

    using Clock = std::chrono::steady_clock;

    HashMap map;
    auto t0 = Clock::now();
    for (auto& rec : subset) map.insert(rec);
    auto mapBuildUs = std::chrono::duration<double, std::micro>(Clock::now() - t0).count();

    Heap heap;
    t0 = Clock::now();
    for (auto& rec : subset) heap.insert(rec);
    auto heapBuildUs = std::chrono::duration<double, std::micro>(Clock::now() - t0).count();

    t0 = Clock::now();
    auto mapResult = map.leastCongestedSlot(day, startHour, endHour);
    auto mapQueryUs = std::chrono::duration<double, std::micro>(Clock::now() - t0).count();

    t0 = Clock::now();
    auto heapResult = heap.leastCongestedSlot(day, startHour, endHour);
    auto heapQueryUs = std::chrono::duration<double, std::micro>(Clock::now() - t0).count();

    if (mapResult.found) {
        hashResultLabel_->setText(QString("HashMap: hour %1, avg %2 (build %3 us, query %4 us)")
            .arg(mapResult.hour)
            .arg(mapResult.avg_headcount, 0, 'f', 2)
            .arg(mapBuildUs, 0, 'f', 1)
            .arg(mapQueryUs, 0, 'f', 1));
    } else {
        hashResultLabel_->setText("HashMap: no matching slot");
    }

    if (heapResult.found) {
        heapResultLabel_->setText(QString("Heap: hour %1, avg %2 (build %3 us, query %4 us)")
            .arg(heapResult.hour)
            .arg(heapResult.avg_headcount, 0, 'f', 2)
            .arg(heapBuildUs, 0, 'f', 1)
            .arg(heapQueryUs, 0, 'f', 1));
    } else {
        heapResultLabel_->setText("Heap: no matching slot");
    }
}
