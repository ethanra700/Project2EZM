#pragma once

#include "GymRecord.h"
#include <QMainWindow>
#include <vector>

class QSpinBox;
class QCheckBox;
class QLabel;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(std::vector<GymRecord> records, QWidget* parent = nullptr);

private slots:
    void runQuery();

private:
    std::vector<GymRecord> records_;

    QSpinBox* daySpin_ = nullptr;
    QSpinBox* startHourSpin_ = nullptr;
    QSpinBox* endHourSpin_ = nullptr;
    QCheckBox* tempFilterCheck_ = nullptr;
    QSpinBox* tempMinSpin_ = nullptr;
    QSpinBox* tempMaxSpin_ = nullptr;
    QLabel* hashResultLabel_ = nullptr;
    QLabel* heapResultLabel_ = nullptr;
};
