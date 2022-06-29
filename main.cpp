#include "mainWindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv); // setup QApplication
    // setup main window
    auto * w = new MainWindow;
    w->show();
    // loop blocking
    return QApplication::exec();
}