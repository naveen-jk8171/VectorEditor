#include <iostream>
#include "io/xml_parser.h"
#include <QApplication>
#include "gui/main_window.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    // std::string filePath = "/Users/naveen_jk8171/Library/CloudStorage/OneDrive-IITDelhi/IITD/SEM_4/COP290/Assignments/0/test.svg";
    // std::string content = readSVGFile(filePath);
    // std::vector<GraphicsObject*> shapes = parse(content);
    MainWindow window;
    window.newFile();
    // window.getCanvas()->loadShapes(shapes);
    window.show();
    return app.exec();
}