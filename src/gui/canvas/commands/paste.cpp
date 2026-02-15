#include "gui/canvas.h"

void Canvas::paste() {
    if (clipboard.empty()) return;
    scene->clearSelection(); // Deselect current items
    std::vector<std::shared_ptr<GraphicsObject>> objs; // Store new objects
    for (auto& shape : clipboard) {
        std::shared_ptr<GraphicsObject> newShape(shape->clone()); // Clone shape
        objs.emplace_back(newShape); // Add to list
        int offset = 20*pasteCount;
        if (auto r = std::dynamic_pointer_cast<Rectangle>(newShape)) {
            r->x += offset; 
            r->y += offset;
        } else if (auto c = std::dynamic_pointer_cast<Circle>(newShape)) {
            c->cx += offset;
            c->cy += offset;
        } else if (auto l = std::dynamic_pointer_cast<Line>(newShape)) {
            l->x1 += offset;
            l->x2 += offset;
            l->y1 += offset; 
            l->y2 += offset;
        } else if (auto h = std::dynamic_pointer_cast<Hexagon>(newShape)) {
            h->cx += offset; 
            h->cy += offset;
        }
        addShape(newShape); // Add shape to canvas
        m_visuals[newShape]->setSelected(true); // Select new shape
    }
    if (!objs.empty()) { // If at least one object was pasted
        auto pasteCommand = std::make_unique<Paste>(this, objs); // Create Paste command
        pushCommand(std::move(pasteCommand)); // Push to undo stack
    }
    updateSelectionHandles(); // Update resize handles
    pasteCount++; // Increment paste count
}