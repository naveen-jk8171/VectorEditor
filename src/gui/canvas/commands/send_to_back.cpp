#include "gui/canvas.h"

void Canvas::sendToBack() {
    if (scene->selectedItems().isEmpty()) return;
    QGraphicsItem* item = scene->selectedItems().first(); // Get selected item
    QVariant data = item->data(0); // get object pointer
    GraphicsObject* objPtr = static_cast<GraphicsObject*>(data.value<void*>()); // Cast pointer
    std::shared_ptr<GraphicsObject> obj = nullptr; // Find shared pointer
    for(auto& s : m_shapes) { if(s.get() == objPtr) { obj = s; break; } } // Search list
    if (obj) {
        auto it = std::find(m_shapes.begin(), m_shapes.end(), obj); // Find position
        if (it != m_shapes.end() && it != m_shapes.begin()) { // If not already at back
            m_shapes.erase(it); // Remove from current position
            m_shapes.insert(m_shapes.begin(), obj); // Insert at beginning (back)
            refreshLayers(); // Update Z-values
            if (m_visuals.find(obj) != m_visuals.end()) {
                m_visuals[obj]->setSelected(true); // Keep selected
            }
        }
    }
}