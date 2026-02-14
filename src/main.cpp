#include <iostream>
#include "io/xml_parser.h"
#include <QApplication>
#include "gui/main_window.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.newFile();
    window.show();
    return app.exec();
}