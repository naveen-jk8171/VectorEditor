#include "gui/canvas.h"

void Canvas::deleteShapes() {
    std::vector<std::shared_ptr<GraphicsObject>> objs;
    for (QGraphicsItem* item : scene->selectedItems()) {
        if (!item || !item->scene()) continue;
        QVariant data = item->data(0);
        GraphicsObject* objPtr = static_cast<GraphicsObject*>(data.value<void*>());
        std::shared_ptr<GraphicsObject> obj = nullptr;
        for(auto& s : m_shapes) { if(s.get() == objPtr) { obj = s; break; } }
        if (obj) {
            removeShape(obj, false);
            objs.emplace_back(obj);
        }
    }
    if (!objs.empty()) {
        auto deleteCommand = std::make_unique<Delete>(this, objs);
        pushCommand(std::move(deleteCommand));
        updateSelectionHandles();
        emit shapeSelected(nullptr);
    }
}

