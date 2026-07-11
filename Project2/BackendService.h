
#pragma once

#include <QObject>
#include <QVariantMap>
#include <qqmlintegration.h>

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
    Q_INVOKABLE QVariantMap weeklyComparison() const;
    Q_INVOKABLE QVariantMap benchmark() const;
    static BackendService *create(QQmlEngine *eng, QJSEngine *js);
};
