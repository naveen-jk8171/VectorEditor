#include "gui/canvas.h"

void Canvas::wheelEvent(QWheelEvent* event) {
    if (event->modifiers() & Qt::ControlModifier) {
        const double sFactor = 1.09;
        if (event->angleDelta().y() > 0) scale(sFactor, sFactor); else scale(1.0/sFactor, 1.0/sFactor);
        event->accept();
    } 
    else {
        QGraphicsView::wheelEvent(event);
    }
}

bool Canvas::viewportEvent(QEvent* event) {
    if (event->type() == QEvent::NativeGesture) {
        QNativeGestureEvent* gesture = static_cast<QNativeGestureEvent*>(event);
        if (gesture->gestureType() == Qt::ZoomNativeGesture) {
            qreal scaleFactor = 1.0 + gesture->value();
            scale(scaleFactor, scaleFactor);
            return true;
        }
    }
    return QGraphicsView::viewportEvent(event);
}