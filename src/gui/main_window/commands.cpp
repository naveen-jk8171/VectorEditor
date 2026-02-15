#include "gui/main_window.h"

void MainWindow::newFile() {
    if (letNew()) {
        canvas->reset();
        filePath = "";
        setWindowTitle("Vector Editor - Untitled");
    }
}

bool MainWindow::saveFile() {
    if (filePath.empty()) {
        return saveAsFile();
    }
    std::vector<std::shared_ptr<GraphicsObject>> rawShapes;
    const auto& shapes = canvas->getShapes();
    for (const auto& s : shapes) rawShapes.push_back(s);
    saveSVGFile(rawShapes, filePath);
    canvas->modified = false;
    setWindowTitle(QString::fromStdString("Vector Editor - " + filePath));
    return true;
}

bool MainWindow::saveAsFile() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save As", "", "SVG Files (*.svg)");
    if (fileName.isEmpty()) return false;
    if (!fileName.endsWith(".svg", Qt::CaseInsensitive)) {
        fileName += ".svg";
    }
    filePath = fileName.toStdString();
    return saveFile();
}

void MainWindow::openFile() {
    if (letNew()) {
        QString fileName = QFileDialog::getOpenFileName(this, "Open File", "", "SVG Files (*.svg)");
        if (!fileName.isEmpty()) {
            filePath = fileName.toStdString();
            std::string content = readSVGFile(filePath);
            std::vector<std::shared_ptr<GraphicsObject>> rawShapes = parse(content);
            canvas->loadShapes(rawShapes);
            setWindowTitle(QString::fromStdString("Vector Editor - " + filePath));
        }
    }
}