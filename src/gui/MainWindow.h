#pragma once

#include "GymRecord.h"
#include "hashmap.h"
#include <QMainWindow>
#include <QString>
#include <QStringList>
#include <vector>

class QLabel;
class QLineEdit;
class QVBoxLayout;
class QWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(std::vector<GymRecord> records, QWidget* parent = nullptr);

private slots:
    void onEnter();

private:
    enum class Step { Choice, Date, Time };

    struct ResultInfo {
        QString heading;
        QString message;
        QStringList details;
    };

    void choose(const QString& num);
    void badInput(const QString& msg);
    void showInfo(const ResultInfo& info, bool isError);
    void updatePrompt();

    bool hasData() const;
    ResultInfo unavailableDataResult() const;
    ResultInfo leastCongestedTimeResult() const;
    ResultInfo datePromptResult() const;
    ResultInfo timePromptResult(const QString& date) const;
    ResultInfo congestionForResult(const QString& date, const QString& time) const;
    ResultInfo weeklyComparisonResult() const;
    ResultInfo benchmarkResult() const;

    static bool dateOk(const QString& val);
    static bool timeOk(const QString& val);
    static QString fixDate(const QString& val);
    static QString fixTime(const QString& val);

    std::vector<GymRecord> records_;
    HashMap slotIndex_;
    double overallAverage_ = 0.0;
    int validRecordCount_ = 0;

    Step step_ = Step::Choice;
    QString date_;

    QLineEdit* choiceEdit_ = nullptr;
    QLabel* promptLabel_ = nullptr;
    QWidget* infoBox_ = nullptr;
    QLabel* infoHeadingLabel_ = nullptr;
    QLabel* infoMessageLabel_ = nullptr;
    QVBoxLayout* infoDetailsLayout_ = nullptr;
};
