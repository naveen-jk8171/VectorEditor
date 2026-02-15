#include "model/rect.h"

void Rectangle::resizeShape(const QPointF& currPos, const QPointF& lastPos, SelectionHandles::HandlePosition handle) {
    double dx = currPos.x()-lastPos.x();
    double dy = currPos.y()-lastPos.y();
    double px = x + width;
    double py = y + height;
    if (handle == SelectionHandles::TOPLEFT) {
        width -= dx;
        height -= dy;
    } else if (handle == SelectionHandles::TOP) {
        height -= dy;
    } else if (handle == SelectionHandles::TOPRIGHT) {
        width += dx;
        height -= dy;
    } else if (handle == SelectionHandles::LEFT) {
        width -= dx;
    } else if (handle == SelectionHandles::RIGHT) {
        width += dx;
    } else if (handle == SelectionHandles::BOTTOMLEFT) {
        width -= dx;
        height += dy;
    } else if (handle == SelectionHandles::BOTTOM) {
        height += dy;
    } else if (handle == SelectionHandles::BOTTOMRIGHT) {
        width += dx;
        height += dy;
    }
    if (width < 10) width = 10;
    if (height < 10) height = 10;
    if (handle == SelectionHandles::TOPLEFT || handle == SelectionHandles::LEFT || handle == SelectionHandles::BOTTOMLEFT) x = px-width;
    if (handle == SelectionHandles::TOPLEFT || handle == SelectionHandles::TOP || handle == SelectionHandles::TOPRIGHT) y = py-height;
}