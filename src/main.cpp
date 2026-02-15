#include <iostream>
#include "io/xml_parser.h"
#include <QApplication>
#include "gui/main_window.h"

int main(int argc, char* argv[]) { // Application entry point
    QApplication app(argc, argv); // Initialize Qt application
    MainWindow window; // Create main window
    window.newFile(); // Initialize new file state
    window.show(); // Show window
    return app.exec(); // Execute event loop
}