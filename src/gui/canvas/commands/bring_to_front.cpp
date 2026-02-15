#include "gui/canvas.h"

void Canvas::bringToFront() {
    if (scene->selectedItems().isEmpty()) return;
    QGraphicsItem* item = scene->selectedItems().first(); // Get the selected item
    QVariant data = item->data(0); // Retrieve stored object pointer
    GraphicsObject* objPtr = static_cast<GraphicsObject*>(data.value<void*>()); // Cast back to GraphicsObject
    std::shared_ptr<GraphicsObject> obj = nullptr; // To hold the shared pointer
    for(auto& s : m_shapes) { if(s.get() == objPtr) { obj = s; break; } } // Find the shared pointer in our list
    if (obj) {
        auto it = std::find(m_shapes.begin(), m_shapes.end(), obj); // Find iterator position
        if (it != m_shapes.end() && it != m_shapes.end()-1) { // If not already at the front
            m_shapes.erase(it); // Remove from current position
            m_shapes.push_back(obj); // Move to end of vector (renders on top)
            refreshLayers(); // Update Z-values for all shapes
            if (m_visuals.find(obj) != m_visuals.end()) {
                m_visuals[obj]->setSelected(true); // Re-select the moved item
            }
        }
    }
}