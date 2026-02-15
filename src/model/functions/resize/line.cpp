#include "model/line.h"

void Line::resizeShape(const QPointF& currPos, const QPointF& lastPos, SelectionHandles::HandlePosition handle) { // Resize logic 
    if (handle == SelectionHandles::LEFT) { // If left handle (start point)
        x1 = currPos.x(); // Update x1
        y1 = currPos.y(); // Update y1
    } else if (handle == SelectionHandles::RIGHT) { // If right handle (end point)
        x2 = currPos.x(); // Update x2
        y2 = currPos.y(); // Update y2
    }
}