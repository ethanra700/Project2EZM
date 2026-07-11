
#pragma once

#include <QObject>
#include <QString>
#include <QVariantMap>
#include <qqmlintegration.h>
#include <vector>

#include "src/data/GymRecord.h"
#include "src/structures/hashmap.h"

class QJSEngine;
class QQmlEngine;

class BackendService : public QObject{
    Q_OBJECT
    QML_NAMED_ELEMENT(Backend)
    QML_SINGLETON
public:
    explicit BackendService(QObject *parent = nullptr);
    Q_INVOKABLE QVariantMap leastCongestedTime() const;
    Q_INVOKABLE QVariantMap datePrompt() const;
    Q_INVOKABLE QVariantMap timePrompt(const QString &date) const;
    Q_INVOKABLE QVariantMap congestionFor(const QString &date, const QString &time) const;
    //Q_INVOKABLE QVariantMap weeklyComparison() const;
    //Q_INVOKABLE QVariantMap benchmark() const;
    static BackendService *create(QQmlEngine *eng, QJSEngine *js);

private:
    bool hasData() const;
    QVariantMap unavailableDataResult() const;

    std::vector<GymRecord> records_;
    HashMap slotIndex_;
    QString dataPath_;
    double overallAverage_ = 0.0;
    int validRecordCount_ = 0;
};
