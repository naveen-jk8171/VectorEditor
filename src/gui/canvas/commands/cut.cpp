#include "gui/canvas.h"

void Canvas::cut() {
    clipboard.clear();
    pasteCount = 1;
    QList<QGraphicsItem*> items = scene->selectedItems();
    std::vector<std::shared_ptr<GraphicsObject>> objs;
    for (auto& item : items) {
        QVariant data = item->data(0);
        GraphicsObject* shapePtr = static_cast<GraphicsObject*>(data.value<void*>());
        std::shared_ptr<GraphicsObject> shape = nullptr;
        for(auto& s : m_shapes) { if(s.get() == shapePtr) { shape = s; break; } }
        if (shape) {
            clipboard.push_back(std::shared_ptr<GraphicsObject>(shape->clone()));
            objs.push_back(shape);
            removeShape(shape, false);
        }
    }
    if (!objs.empty()) {
        auto cutCommand = std::make_unique<Cut>(this, objs);
        pushCommand(std::move(cutCommand));
    }
}