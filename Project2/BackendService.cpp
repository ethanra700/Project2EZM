
#include "BackendService.h"

#include <QJSEngine>
#include <QQmlEngine>
#include <QStringList>
#include <QVariantList>

namespace {
QVariantMap result(const QString &heading, const QString &message, const QStringList &details){
    return {{"heading", heading},{"message", message},{"details", QVariant::fromValue(details)}};
    }
}

BackendService::BackendService(QObject *parent): QObject(parent){}

BackendService *BackendService::create(QQmlEngine *eng, QJSEngine *js){
    Q_UNUSED(eng)
    Q_UNUSED(js)
    return new BackendService;
}
QVariantMap BackendService::leastCongestedTime() const{
    // Replace these sample values only temp here for demoing purposes
    return result("Least Congested Time Today","10:30 AM is currently the lowest-congestion window.",{"Estimated wait: 4 minutes","Sample value from BackendService.cpp"});
}

QVariantMap BackendService::datePrompt() const{
    return result("Check Congestion","Enter a date without a year to continue.",{"Format: MM/DD", "Example: 07/10"});
}

QVariantMap BackendService::timePrompt(const QString &date) const{
    return result("Check Congestion - " + date,"Now enter the time for that date.",{"Format: h:mm AM/PM", "Example: 5:30 PM"});
}

QVariantMap BackendService::congestionFor(const QString &date, const QString &time) const{
    //replace
    const QString key = date + "|" + time;
    if (key == "07/10|8:30 AM") {
        return result("Congestion for " + date + " at " + time,"Low congestion",{"Estimated wait: About 4 minutes","Sample backend record"});
    }
    if (key == "07/10|5:30 PM") {
        return result("Congestion for " + date + " at " + time,"High congestion",{"Estimated wait: About 18 minutes","Sample backend record"});
    }
    return result("Congestion for " + date + " at " + time,"No matching backend record is available yet.",{"Add your C++ lookup in BackendService::congestionFor()."});
}

QVariantMap BackendService::weeklyComparison() const{
    //replace
    return result("Weekly Congestion Comparison","Tuesday has the lowest average congestion in this sample dataset.",{"Best day: Tuesday - 28% average occupancy","Busiest day: Thursday - 76% average occupancy"});
}
QVariantMap BackendService::benchmark() const{
    //replace with real results
    return result("Benchmark: Heap vs Hash Map", "Sample benchmark results are ready to replace with C++ measurements.",{"Heap lookup: 1.8 ms", "Hash map lookup: 0.5 ms"});
}
