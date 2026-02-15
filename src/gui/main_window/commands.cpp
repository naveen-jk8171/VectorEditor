#include "gui/main_window.h"

void MainWindow::newFile() {
    if (letNew()) { // Check unsaved changes
        canvas->reset(); // Clear the canvas
        filePath = ""; // Reset file path
        setWindowTitle("Vector Editor - Untitled"); // Update window title
    }
}

bool MainWindow::saveFile() {
    if (filePath.empty()) {
        return saveAsFile(); // Trigger Save As if not filePath is there
    }
    std::vector<std::shared_ptr<GraphicsObject>> rawShapes = canvas->getShapes(); // Get shapes from canvas
    saveSVGFile(rawShapes, filePath); // Write to SVG 
    canvas->modified = false; // Reset modified flag
    setWindowTitle(QString::fromStdString("Vector Editor - " + filePath)); // Update title
    return true;
}

bool MainWindow::saveAsFile() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save As", "", "SVG Files (*.svg)"); // Open dialog
    if (fileName.isEmpty()) return false; // User cancelled
    if (!fileName.endsWith(".svg", Qt::CaseInsensitive)) {
        fileName += ".svg";
    }
    filePath = fileName.toStdString();
    return saveFile();
}

void MainWindow::openFile() {
    if (letNew()) { // Check for unsaved changes
        QString fileName = QFileDialog::getOpenFileName(this, "Open File", "", "SVG Files (*.svg)"); // Open dialog
        if (!fileName.isEmpty()) { // User selected file
            filePath = fileName.toStdString();
            std::string content = readSVGFile(filePath); // Read file content 
            std::vector<std::shared_ptr<GraphicsObject>> rawShapes = parse(content);
            canvas->loadShapes(rawShapes);
            setWindowTitle(QString::fromStdString("Vector Editor - " + filePath)); // Title update
        }
    }
}