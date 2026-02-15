#include "gui/canvas.h"

void Canvas::paste() {
    if (clipboard.empty()) return;
    scene->clearSelection();
    std::vector<std::shared_ptr<GraphicsObject>> objs;
    for (auto& shape : clipboard) {
        std::shared_ptr<GraphicsObject> newShape(shape->clone());
        objs.emplace_back(newShape);
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
        addShape(newShape);
        m_visuals[newShape]->setSelected(true);
    }
    if (!objs.empty()) {
        auto pasteCommand = std::make_unique<Paste>(this, objs);
        pushCommand(std::move(pasteCommand));
    }
    updateSelectionHandles();
    pasteCount++;
}