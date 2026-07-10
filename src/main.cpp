#include "MainWindow.h"
#include "bench.h"
#include <QApplication>

int main(int argc, char** argv) {
    runBenchmarks();

    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
