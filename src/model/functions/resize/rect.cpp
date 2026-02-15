#include "model/rect.h"

void Rectangle::resizeShape(const QPointF& currPos, const QPointF& lastPos, SelectionHandles::HandlePosition handle) { // Resize logic 
    double dx = currPos.x()-lastPos.x(); // Delta X
    double dy = currPos.y()-lastPos.y(); // Delta Y
    double px = x + width; // Pivot X (Right)
    double py = y + height; // Pivot Y (Bottom)
    if (handle == SelectionHandles::TOPLEFT) { // Top-Left handle
        width -= dx; // Adjust width
        height -= dy; // Adjust height
    } else if (handle == SelectionHandles::TOP) { // Top handle
        height -= dy; // Adjust height
    } else if (handle == SelectionHandles::TOPRIGHT) { // Top-Right handle
        width += dx; // Adjust width
        height -= dy; // Adjust height
    } else if (handle == SelectionHandles::LEFT) { // Left handle
        width -= dx; // Adjust width
    } else if (handle == SelectionHandles::RIGHT) { // Right handle
        width += dx; // Adjust width
    } else if (handle == SelectionHandles::BOTTOMLEFT) { // Bottom-Left handle
        width -= dx; // Adjust width
        height += dy; // Adjust height
    } else if (handle == SelectionHandles::BOTTOM) { // Bottom handle
        height += dy; // Adjust height
    } else if (handle == SelectionHandles::BOTTOMRIGHT) { // Bottom-Right handle
        width += dx; // Adjust width
        height += dy; // Adjust height
    }
    if (width < 10) width = 10; // Min width
    if (height < 10) height = 10; // Min height
    if (handle == SelectionHandles::TOPLEFT || handle == SelectionHandles::LEFT || handle == SelectionHandles::BOTTOMLEFT) x = px-width; // Correct X pos
    if (handle == SelectionHandles::TOPLEFT || handle == SelectionHandles::TOP || handle == SelectionHandles::TOPRIGHT) y = py-height; // Correct Y pos
}