#include "gui/canvas.h"

void Canvas::keyPressEvent(QKeyEvent* event) {
    QGraphicsItem* focusItem = scene->focusItem(); // Get the currently focused item
    if (focusItem) {
        QGraphicsTextItem* ti = dynamic_cast<QGraphicsTextItem*>(focusItem);
        if (ti && (ti->textInteractionFlags() & Qt::TextEditorInteraction)) { // If text editing is active
            QGraphicsView::keyPressEvent(event); // Let default handler handle text input
            return;
        }
    }
    if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace) {
        deleteShapes(); // Call delete function
    } else {
        QGraphicsView::keyPressEvent(event); // Pass other keys to default handler
    }
}