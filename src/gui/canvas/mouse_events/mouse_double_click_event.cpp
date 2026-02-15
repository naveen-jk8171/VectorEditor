#include "gui/canvas.h"

void Canvas::mouseDoubleClickEvent(QMouseEvent* event) {
    QGraphicsItem* item = itemAt(event->pos()); // Get item under cursor
    if (item) { // If there is an item
        QGraphicsTextItem* ti = dynamic_cast<QGraphicsTextItem*>(item);
        if (ti) { // If it is text
            QVariant data = ti->data(0); // Get object pointer
            GraphicsObject* objPtr = static_cast<GraphicsObject*>(data.value<void*>()); // Cast pointer
            std::shared_ptr<GraphicsObject> obj = nullptr; // Find shared pointer
            for(auto& s : m_shapes) { if(s.get() == objPtr) { obj = s; break; } } // Search list
            auto t = std::dynamic_pointer_cast<Text>(obj); // Cast to Text model
            if (t) {
                t->showBorder = true; // Enable border for editing
                updateShape(t);
                item = m_visuals[t]; // Get new visual item
                ti = dynamic_cast<QGraphicsTextItem*>(item); // Cast again
            }
            ti->setTextInteractionFlags(Qt::TextEditorInteraction); // Enable editing
            ti->setFocus(); // Set focus to item
            QGraphicsView::mouseDoubleClickEvent(event);
            return;
        }
    }
}