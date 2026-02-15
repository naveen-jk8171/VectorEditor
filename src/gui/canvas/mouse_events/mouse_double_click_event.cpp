#include "gui/canvas.h"

void Canvas::mouseDoubleClickEvent(QMouseEvent* event) {
    QGraphicsItem* item = itemAt(event->pos());
    if (item) {
        QGraphicsTextItem* ti = dynamic_cast<QGraphicsTextItem*>(item);
        if (ti) {
            QVariant data = ti->data(0);
            GraphicsObject* objPtr = static_cast<GraphicsObject*>(data.value<void*>());
            std::shared_ptr<GraphicsObject> obj = nullptr;
            for(auto& s : m_shapes) { if(s.get() == objPtr) { obj = s; break; } }
            auto t = std::dynamic_pointer_cast<Text>(obj);
            if (t) {
                t->showBorder = true;
                updateShape(t);
                item = m_visuals[t];
                ti = dynamic_cast<QGraphicsTextItem*>(item);
            }
            ti->setTextInteractionFlags(Qt::TextEditorInteraction);
            ti->setFocus();
            QGraphicsView::mouseDoubleClickEvent(event);
            return;
        }
    }
}