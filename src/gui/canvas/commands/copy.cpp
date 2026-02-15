#include "gui/canvas.h"

void Canvas::copy() {
    clipboard.clear(); // Clear previous clipboard content
    pasteCount = 1; // Reset paste offset counter
    QList<QGraphicsItem*> items = scene->selectedItems(); // Get all selected items
    for (auto& item : items) {
        QVariant data = item->data(0); // Get object pointer
        GraphicsObject* shapePtr = static_cast<GraphicsObject*>(data.value<void*>()); // Cast to GraphicsObject
        std::shared_ptr<GraphicsObject> shape = nullptr; // Holder for shared pointer
        for(auto& s : m_shapes) { if(s.get() == shapePtr) { shape = s; break; } } // Find actual shape object
        if (shape) { // If valid shape found
            clipboard.push_back(std::shared_ptr<GraphicsObject>(shape->clone())); // Clone and add to clipboard
        }
    }
}