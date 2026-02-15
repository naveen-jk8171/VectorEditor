#include "model/line.h"

void Line::resizeShape(const QPointF& currPos, const QPointF& lastPos, SelectionHandles::HandlePosition handle) {
    if (handle == SelectionHandles::LEFT) {
        x1 = currPos.x();
        y1 = currPos.y();
    } else if (handle == SelectionHandles::RIGHT) {
        x2 = currPos.x();
        y2 = currPos.y();
    }
}