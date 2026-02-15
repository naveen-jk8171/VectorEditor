#include "gui/main_window.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    canvas = new Canvas(this);
    setCentralWidget(canvas);
    resize(1200, 800);
    setWindowTitle("Vector Editor");

    QMenu* fileMenu = menuBar()->addMenu("&File");
    newAction = fileMenu->addAction("New File...");
    newAction->setShortcut(QKeySequence::New);
    openAction = fileMenu->addAction("Open...");
    openAction->setShortcut(QKeySequence::Open);
    fileMenu->addSeparator();
    saveAction = fileMenu->addAction("Save");
    saveAction->setShortcut(QKeySequence::Save);
    saveAsAction = fileMenu->addAction("Save As...");
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    fileMenu->addSeparator();
    exitAction = fileMenu->addAction("Exit");

    QMenu* editMenu = menuBar()->addMenu("Edit");
    undoAction = editMenu->addAction("Undo");
    undoAction->setShortcut(QKeySequence::Undo);
    redoAction = editMenu->addAction("Redo");
    redoAction->setShortcut(QKeySequence::Redo);
    editMenu->addSeparator();
    cutAction = editMenu->addAction("Cut");
    cutAction->setShortcut(QKeySequence::Cut);
    copyAction = editMenu->addAction("Copy");
    copyAction->setShortcut(QKeySequence::Copy);
    pasteAction = editMenu->addAction("Paste");
    pasteAction->setShortcut(QKeySequence::Paste);
    selectAllAction = editMenu->addAction("Select All");
    selectAllAction->setShortcut(QKeySequence::SelectAll);
    deleteAction = editMenu->addAction("Delete");
    deleteAction->setShortcut(QKeySequence::Delete);
    bringToFrontAction = editMenu->addAction("Front");
    sendToBackAction = editMenu->addAction("Back");

    QToolBar* toolbar = addToolBar("Tools");
    addToolBar(Qt::LeftToolBarArea, toolbar);
    toolbar->setMovable(false);
    toolbar->setIconSize(QSize(32, 32));
    
    selectAction = toolbar->addAction("↖");
    selectAction->setCheckable(true);

    addRectangleAction = toolbar->addAction("⬜");
    addRectangleAction->setCheckable(true);

    addCircleAction = toolbar->addAction("◯");
    addCircleAction->setCheckable(true);

    addLineAction = toolbar->addAction("╱");
    addLineAction->setCheckable(true);

    addHexagonAction = toolbar->addAction("Hex");
    addHexagonAction->setCheckable(true);

    addFreeHandAction = toolbar->addAction("Free");
    addFreeHandAction->setCheckable(true);

    addTextAction = toolbar->addAction("Text");
    addTextAction->setCheckable(true);

    QActionGroup* toolGroup = new QActionGroup(this);
    toolGroup->addAction(selectAction);
    toolGroup->addAction(addRectangleAction);
    toolGroup->addAction(addCircleAction);
    toolGroup->addAction(addLineAction);
    toolGroup->addAction(addHexagonAction);
    toolGroup->addAction(addFreeHandAction);
    toolGroup->addAction(addTextAction);
    selectAction->setChecked(true);

    QDockWidget* dock = new QDockWidget("Properties", this);
    dock->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    dock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);

    panel = new PropertyPanel(canvas, dock);
    dock->setWidget(panel);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    connections();
}