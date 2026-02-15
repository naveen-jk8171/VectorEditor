#include "gui/canvas.h"

void Canvas::deleteShapes() {
    std::vector<std::shared_ptr<GraphicsObject>> objs; // Store objects to delete
    for (QGraphicsItem* item : scene->selectedItems()) { // Iterate selected items
        if (!item || !item->scene()) continue; // Skip invalid items
        QVariant data = item->data(0); // Get object pointer
        GraphicsObject* objPtr = static_cast<GraphicsObject*>(data.value<void*>()); // Cast pointer
        std::shared_ptr<GraphicsObject> obj = nullptr; // Find shared pointer
        for(auto& s : m_shapes) { if(s.get() == objPtr) { obj = s; break; } } // Search list
        if (obj) { // If object found
            removeShape(obj, false); // Remove from canvas
            objs.emplace_back(obj); // Add to list for command
        }
    }
    if (!objs.empty()) { // if at least one object was deleted
        auto deleteCommand = std::make_unique<Delete>(this, objs); // Create Delete command
        pushCommand(std::move(deleteCommand)); // Push to undo stack
        updateSelectionHandles();
        emit shapeSelected(nullptr);
    }
}