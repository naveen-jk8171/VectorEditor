#include "gui/main_window.h"

void MainWindow::connections() {
    connect(newAction, &QAction::triggered, this, &MainWindow::newFile);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveAsFile);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    connect(undoAction, &QAction::triggered, canvas, &Canvas::undo);
    connect(redoAction, &QAction::triggered, canvas, &Canvas::redo);
    connect(cutAction, &QAction::triggered, canvas, &Canvas::cut);
    connect(copyAction, &QAction::triggered, canvas, &Canvas::copy);
    connect(pasteAction, &QAction::triggered, canvas, &Canvas::paste);
    connect(selectAllAction, &QAction::triggered, canvas, &Canvas::selectAll);
    connect(deleteAction, &QAction::triggered, canvas, &Canvas::deleteShapes);
    connect(bringToFrontAction, &QAction::triggered, canvas, &Canvas::bringToFront);
    connect(sendToBackAction, &QAction::triggered, canvas, &Canvas::sendToBack);
    connect(selectAction, &QAction::triggered, this, [this]() {
            canvas->setTool(ToolType::SELECT);
        });
    connect(addRectangleAction, &QAction::triggered, this, [this]() {
            canvas->getScene()->clearSelection();
            canvas->updateSelectionHandles();
            canvas->setTool(ToolType::RECTANGLE);
        });
    connect(addCircleAction, &QAction::triggered, this, [this]() {
            canvas->getScene()->clearSelection();
            canvas->updateSelectionHandles();
            canvas->setTool(ToolType::CIRCLE);
        });
    connect(addLineAction, &QAction::triggered, this, [this]() {
            canvas->getScene()->clearSelection();
            canvas->updateSelectionHandles();
            canvas->setTool(ToolType::LINE);
        });
    connect(addHexagonAction, &QAction::triggered, this, [this]() {
            canvas->getScene()->clearSelection();
            canvas->updateSelectionHandles();
            canvas->setTool(ToolType::HEXAGON);
        });
    connect(addFreeHandAction, &QAction::triggered, this, [this]() {
            canvas->getScene()->clearSelection();
            canvas->updateSelectionHandles();
            canvas->setTool(ToolType::FREEHAND);
        });
    connect(addTextAction, &QAction::triggered, this, [this] () {
            canvas->getScene()->clearSelection();
            canvas->updateSelectionHandles();
            canvas->setTool(ToolType::TEXT);
        });
    connect(canvas, &Canvas::shapeSelected, this, [this](std::shared_ptr<GraphicsObject> shape) {
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
    connect(panel, &PropertyPanel::commandGenerated, this, [this](Command* commandPtr) {
            canvas->pushCommand(std::unique_ptr<Command>(commandPtr));
        });
}