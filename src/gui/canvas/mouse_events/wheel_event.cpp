#include "gui/canvas.h"

void Canvas::wheelEvent(QWheelEvent* event) {
    if (event->modifiers() & Qt::ControlModifier) { // Check for Ctrl key
        const double sFactor = 1.09; // Zoom factor
        if (event->angleDelta().y() > 0) scale(sFactor, sFactor); else scale(1.0/sFactor, 1.0/sFactor); // Zoom in/out
        event->accept();
    } 
    else {
        QGraphicsView::wheelEvent(event); // default handling (for scrolling)
    }
}

bool Canvas::viewportEvent(QEvent* event) {
    if (event->type() == QEvent::NativeGesture) { // Handle trackpad gestures
        QNativeGestureEvent* gesture = static_cast<QNativeGestureEvent*>(event);
        if (gesture->gestureType() == Qt::ZoomNativeGesture) { // Pinch to zoom in ro out
            qreal scaleFactor = 1.0 + gesture->value();
            scale(scaleFactor, scaleFactor);
            return true;
        }
    }
    return QGraphicsView::viewportEvent(event);
}