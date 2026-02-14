#include "gui/main_window.h"
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

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    canvas = new Canvas(this);
    setCentralWidget(canvas);
    resize(1200, 800);
    setWindowTitle("Vector Editor");

    QMenu* fileMenu = menuBar()->addMenu("&File");
    QAction* newAction = fileMenu->addAction("New File...");
    newAction->setShortcut(QKeySequence::New);
    connect(newAction, &QAction::triggered, this, &MainWindow::newFile);
    QAction* openAction = fileMenu->addAction("Open...");
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    fileMenu->addSeparator();
    QAction* saveAction = fileMenu->addAction("Save");
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);
    QAction* saveAsAction = fileMenu->addAction("Save As...");
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveAsFile);
    fileMenu->addSeparator();
    QAction* exitAction = fileMenu->addAction("Exit");
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    QMenu* editMenu = menuBar()->addMenu("Edit");
    QAction* undoAction = editMenu->addAction("Undo");
    undoAction->setShortcut(QKeySequence::Undo);
    connect(undoAction, &QAction::triggered, canvas, &Canvas::undo);
    QAction* redoAction = editMenu->addAction("Redo");
    redoAction->setShortcut(QKeySequence::Redo);
    connect(redoAction, &QAction::triggered, canvas, &Canvas::redo);
    editMenu->addSeparator();
    QAction* cutAction = editMenu->addAction("Cut");
    cutAction->setShortcut(QKeySequence::Cut);
    connect(cutAction, &QAction::triggered, canvas, &Canvas::cut);
    QAction* copyAction = editMenu->addAction("Copy");
    copyAction->setShortcut(QKeySequence::Copy);
    connect(copyAction, &QAction::triggered, canvas, &Canvas::copy);
    QAction* pasteAction = editMenu->addAction("Paste");
    pasteAction->setShortcut(QKeySequence::Paste);
    connect(pasteAction, &QAction::triggered, canvas, &Canvas::paste);
    QAction* selectAllAction = editMenu->addAction("Select All");
    selectAllAction->setShortcut(QKeySequence::SelectAll);
    connect(selectAllAction, &QAction::triggered, canvas, &Canvas::selectAll);
    QAction* deleteAction = editMenu->addAction("Delete");
    deleteAction->setShortcut(QKeySequence::Delete);
    connect(deleteAction, &QAction::triggered, canvas, &Canvas::deleteShapes);
    QAction* bringToFrontAction = editMenu->addAction("Front");
    connect(bringToFrontAction, &QAction::triggered, canvas, &Canvas::bringToFront);
    QAction* sendToBackAction = editMenu->addAction("Back");
    connect(sendToBackAction, &QAction::triggered, canvas, &Canvas::sendToBack);

    QToolBar* toolbar = addToolBar("Tools");
    addToolBar(Qt::LeftToolBarArea, toolbar);
    toolbar->setMovable(false);
    toolbar->setIconSize(QSize(32, 32));
    
    selectAction = toolbar->addAction("↖");
    selectAction->setCheckable(true);
    connect(selectAction, &QAction::triggered, this, [this, toolbar]() {
        canvas->setTool(ToolType::SELECT);
    });

    QAction* addRectangleAction = toolbar->addAction("⬜");
    addRectangleAction->setCheckable(true);
    connect(addRectangleAction, &QAction::triggered, this, [this, toolbar]() {
        canvas->getScene()->clearSelection();
        canvas->updateSelectionHandles();
        canvas->setTool(ToolType::RECTANGLE);
    });

    QAction* addCircleAction = toolbar->addAction("◯");
    addCircleAction->setCheckable(true);
    connect(addCircleAction, &QAction::triggered, this, [this, toolbar]() {
        canvas->getScene()->clearSelection();
        canvas->updateSelectionHandles();
        canvas->setTool(ToolType::CIRCLE);
    });

    QAction* addLineAction = toolbar->addAction("╱");
    addLineAction->setCheckable(true);
    connect(addLineAction, &QAction::triggered, this, [this, toolbar]() {
        canvas->getScene()->clearSelection();
        canvas->updateSelectionHandles();
        canvas->setTool(ToolType::LINE);
    });

    QAction* addHexagonAction = toolbar->addAction("Hex");
    addHexagonAction->setCheckable(true);
    connect(addHexagonAction, &QAction::triggered, this, [this, toolbar]() {
        canvas->getScene()->clearSelection();
        canvas->updateSelectionHandles();
        canvas->setTool(ToolType::HEXAGON);
    });

    QAction* addFreeHandAction = toolbar->addAction("Free");
    addFreeHandAction->setCheckable(true);
    connect(addFreeHandAction, &QAction::triggered, this, [this, toolbar]() {
        canvas->getScene()->clearSelection();
        canvas->updateSelectionHandles();
        canvas->setTool(ToolType::FREEHAND);
    });

    QAction* addTextAction = toolbar->addAction("Text");
    addTextAction->setCheckable(true);
    connect(addTextAction, &QAction::triggered, this, [this, toolbar] () {
        canvas->getScene()->clearSelection();
        canvas->updateSelectionHandles();
        canvas->setTool(ToolType::TEXT);
    });

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

    PropertyPanel* panel = new PropertyPanel(canvas, dock);
    dock->setWidget(panel);
    addDockWidget(Qt::RightDockWidgetArea, dock);
    connect(canvas, &Canvas::shapeSelected, this, [panel](std::shared_ptr<GraphicsObject> shape) {
        panel->setTargetShape(shape.get());
    });
    connect(panel, &PropertyPanel::propertyChanged, this, [this](GraphicsObject* shapePtr){
        std::shared_ptr<GraphicsObject> shape = nullptr;
        const auto& shapes = canvas->getShapes();
        for(const auto& s : shapes) { if(s.get() == shapePtr) { shape = s; break; } }
        if(shape) {
            canvas->updateShape(shape);
            canvas->updateSelectionHandles();
        }
    });
    // Updated connection to handle pointer transfer safely in Qt signal/slot context
    connect(panel, &PropertyPanel::commandGenerated, this, [this](Command* commandPtr) {
        canvas->pushCommand(std::unique_ptr<Command>(commandPtr));
    });
}

Canvas* MainWindow::getCanvas() {
    return canvas;
}

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

bool MainWindow::letNew() {
    if (canvas->modified) {
        QMessageBox::StandardButton response = QMessageBox::warning(this, "Vector Editor", "The document has been modified.\nDo you want to save your changes?", QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (response == QMessageBox::Save) {
            return saveFile();
        } else if (response == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

void MainWindow::closeEvent(QCloseEvent* event) {
    if (letNew()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        selectAction->trigger();
    } else {
        QMainWindow::keyPressEvent(event);
    }
}