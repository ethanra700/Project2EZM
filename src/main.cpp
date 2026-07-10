#include "MainWindow.h"
#include "bench.h"
#include "loader.h"
#include <QApplication>

int main(int argc, char** argv) {
    auto records = loadRecords("data.csv");
    runBenchmarks(records);

    QApplication app(argc, argv);
    MainWindow window(records);
    window.show();
    return app.exec();
}
