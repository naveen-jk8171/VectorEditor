#include "gui/canvas.h"

void Canvas::selectAll() {
    for (auto& obj : m_shapes) m_visuals[obj]->setSelected(true); // Select visual items
    updateSelectionHandles(); // Show handles for selection
}

void Canvas::reset() {
    current_drawing_object = nullptr; // Clear current drawing state
    if (selectionHandles) { // If handles exist
        selectionHandles.reset(); // Destroy handles
    }
    m_shapes.clear(); // Clear all shapes
    m_visuals.clear(); // Clear visual mapping
    scene->clear(); // Clear graphics scene
    modified = false; // Reset modification flag
}

void Canvas::setTool(ToolType tool) {
    current_tool = tool; // Update current tool
    if (tool == ToolType::SELECT) {
        this->setDragMode(QGraphicsView::RubberBandDrag); // Enable selection box
        this->setCursor(Qt::ArrowCursor);
    } else { // If drawing tool
        this->setDragMode(QGraphicsView::NoDrag); // Disable selection box
        this->setCursor(Qt::CrossCursor); // Set crosshair cursor
    }
}

void Canvas::pushCommand(std::unique_ptr<Command> command) {
    undoStack.push(std::move(command)); // Push command to undo stack
    while (!redoStack.empty()) {
        redoStack.pop(); // Remove redo items
    }
}

void Canvas::redraw() {
    scene->clear(); // Clear the scene
    m_visuals.clear(); // Clear visual map
    for (auto& obj : m_shapes) { // Iterate all shapes
        QGraphicsItem* item = obj->draw(scene.get()); // Draw shape
        m_visuals[obj] = item; // Map object to item
    }
}

void Canvas::refreshLayers() {
    for (int i = 0; i < (int)m_shapes.size(); i++) { // Loop through shapes
        m_visuals[m_shapes[i]]->setZValue(i); // Set Z-value based on list index
    }
}

int Canvas::getCountItems() {
    return (int)m_shapes.size(); // total shape count
}

QGraphicsItem* Canvas::getVisualItem(std::shared_ptr<GraphicsObject> obj) const {
    if (m_visuals.count(obj)) {
        return m_visuals.at(obj); // Return visual item
    }
    return nullptr; // if not found
}