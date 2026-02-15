#include "gui/canvas.h"

void Canvas::bringToFront() {
    if (scene->selectedItems().isEmpty()) return;
    QGraphicsItem* item = scene->selectedItems().first();
    QVariant data = item->data(0);
    GraphicsObject* objPtr = static_cast<GraphicsObject*>(data.value<void*>());
    std::shared_ptr<GraphicsObject> obj = nullptr;
    for(auto& s : m_shapes) { if(s.get() == objPtr) { obj = s; break; } }
    if (obj) {
        auto it = std::find(m_shapes.begin(), m_shapes.end(), obj);
        if (it != m_shapes.end() && it != m_shapes.end()-1) {
            m_shapes.erase(it);
            m_shapes.push_back(obj);
            refreshLayers();
            if (m_visuals.find(obj) != m_visuals.end()) {
                m_visuals[obj]->setSelected(true);
            }
        }
    }
}