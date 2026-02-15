#include "gui/canvas.h"

void Canvas::cut() {
    clipboard.clear(); // Clear clipboard for new cut
    pasteCount = 1; // Reset paste offset
    QList<QGraphicsItem*> items = scene->selectedItems(); // Get selected items
    std::vector<std::shared_ptr<GraphicsObject>> objs; // Store objects to be cut
    for (auto& item : items) {
        QVariant data = item->data(0); // Retrieve object data
        GraphicsObject* shapePtr = static_cast<GraphicsObject*>(data.value<void*>()); // Cast pointer
        std::shared_ptr<GraphicsObject> shape = nullptr; // Find shared pointer
        for(auto& s : m_shapes) { if(s.get() == shapePtr) { shape = s; break; } } // Search in m_shapes
        if (shape) { // If shape exists
            clipboard.push_back(std::shared_ptr<GraphicsObject>(shape->clone())); // Add copy to clipboard
            objs.push_back(shape); // Add to list for undo command
            removeShape(shape, false); // Remove from canvas
        }
    }
    if (!objs.empty()) { // If anything was cut
        auto cutCommand = std::make_unique<Cut>(this, objs); // Create Cut command
        pushCommand(std::move(cutCommand)); // Push to undo stack
    }
}