#pragma once

#include <QMainWindow>
#include "gui/canvas.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    Canvas* getCanvas();
    void newFile();
    void openFile();
    bool saveFile();
    bool saveAsFile();
protected:
    void closeEvent(QCloseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
private:
    bool letNew();
    Canvas* canvas;
    std::string filePath;
    QAction* selectAction;
};