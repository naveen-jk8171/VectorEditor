#include "gui/canvas.h"

void Canvas::keyPressEvent(QKeyEvent* event) {
    QGraphicsItem* focusItem = scene->focusItem();
    if (focusItem) {
        QGraphicsTextItem* ti = dynamic_cast<QGraphicsTextItem*>(focusItem);
        if (ti && (ti->textInteractionFlags() & Qt::TextEditorInteraction)) {
            QGraphicsView::keyPressEvent(event);
            return;
        }
    }
    if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace) {
        deleteShapes();
    } else {
        QGraphicsView::keyPressEvent(event);
    }
}