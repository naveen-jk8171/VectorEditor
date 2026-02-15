#pragma once

#include <QMainWindow>
#include <memory>
#include "gui/canvas.h"
#include <QToolBar>
#include <QAction>
#include <QGraphicsScene>
#include <QActionGroup>
#include <QDockWidget>
#include <QMenuBar>
#include "model/rect.h"
#include "model/circle.h"
#include "model/line.h"
#include "io/svg_writer.h"
#include "gui/property_panel.h"
#include "io/xml_parser.h"
#include <QFileDialog>
#include <QMessageBox>

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
    void connections();
    Canvas* canvas;
    std::string filePath;
    QAction* selectAction;
    QAction* newAction;
    QAction* openAction;
    QAction* saveAction;
    QAction* saveAsAction;
    QAction* exitAction;
    QAction* undoAction;
    QAction* redoAction;
    QAction* cutAction;
    QAction* copyAction;
    QAction* pasteAction;
    QAction* selectAllAction;
    QAction* deleteAction;
    QAction* bringToFrontAction;
    QAction* sendToBackAction;
    QAction* addRectangleAction;
    QAction* addCircleAction;
    QAction* addLineAction;
    QAction* addHexagonAction;
    QAction* addFreeHandAction;
    QAction* addTextAction;
    PropertyPanel* panel;
};