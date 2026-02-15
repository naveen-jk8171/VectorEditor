#include "gui/canvas.h"

void Canvas::copy() {
    clipboard.clear();
    pasteCount = 1;
    QList<QGraphicsItem*> items = scene->selectedItems();
    for (auto& item : items) {
        QVariant data = item->data(0);
        GraphicsObject* shapePtr = static_cast<GraphicsObject*>(data.value<void*>());
        std::shared_ptr<GraphicsObject> shape = nullptr;
        for(auto& s : m_shapes) { if(s.get() == shapePtr) { shape = s; break; } }
        if (shape) {
            clipboard.push_back(std::shared_ptr<GraphicsObject>(shape->clone()));
        }
    }
}