#include "gui/canvas.h"

void Canvas::selectAll() {
    for (auto& obj : m_shapes) m_visuals[obj]->setSelected(true);
    updateSelectionHandles();
}

void Canvas::reset() {
    current_drawing_object = nullptr;
    if (selectionHandles) {
        selectionHandles.reset();
    }
    m_shapes.clear();
    m_visuals.clear();
    scene->clear();
    modified = false;
}

void Canvas::setTool(ToolType tool) {
    current_tool = tool;
    if (tool == ToolType::SELECT) {
        this->setDragMode(QGraphicsView::RubberBandDrag);
        this->setCursor(Qt::ArrowCursor);
    } else {
        this->setDragMode(QGraphicsView::NoDrag);
        this->setCursor(Qt::CrossCursor);
    }
}

void Canvas::pushCommand(std::unique_ptr<Command> command) {
    undoStack.push(std::move(command));
    while (!redoStack.empty()) {
        redoStack.pop();
    }
}

void Canvas::redraw() {
    scene->clear();
    m_visuals.clear();
    for (auto& obj : m_shapes) {
        QGraphicsItem* item = obj->draw(scene.get());
        m_visuals[obj] = item;
    }
}

void Canvas::refreshLayers() {
    for (int i = 0; i < (int)m_shapes.size(); i++) {
        m_visuals[m_shapes[i]]->setZValue(i);
    }
}

int Canvas::getCountItems() {
    return (int)m_shapes.size();
}

QGraphicsItem* Canvas::getVisualItem(std::shared_ptr<GraphicsObject> obj) const {
    if (m_visuals.count(obj)) {
        return m_visuals.at(obj);
    }
    return nullptr;
}