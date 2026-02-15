#include "gui/main_window.h"

Canvas* MainWindow::getCanvas() {
    return canvas;
}

bool MainWindow::letNew() { 
    if (canvas->modified) {
        QMessageBox::StandardButton response = QMessageBox::warning(this, "Vector Editor", "The document has been modified.\nDo you want to save your changes?", QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (response == QMessageBox::Save) {
            return saveFile(); // try save
        } else if (response == QMessageBox::Cancel) {
            return false; // on cancel
        }
    }
    return true;
}

void MainWindow::closeEvent(QCloseEvent* event) {
    if (letNew()) event->accept(); else event->ignore(); // Ccheck save
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) selectAction->trigger(); else QMainWindow::keyPressEvent(event); // Esc to Select
}