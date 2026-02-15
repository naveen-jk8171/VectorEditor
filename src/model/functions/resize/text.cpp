#include "model/text.h"

void Text::resizeShape(const QPointF& currPos, const QPointF& lastPos, SelectionHandles::HandlePosition handle) { // Resize logic 
    double dx = currPos.x()-lastPos.x(); // Delta X
    double dy = currPos.y()-lastPos.y(); // Delta Y
    double px = x + width; // Pivot X
    double py = y + height; // Pivot Y
    int lineCount = visualItem->document()->lineCount(); // Get line count
    if (handle == SelectionHandles::TOPLEFT) { // Top-Left
        width -= dx; // Adjust width
        height -= dy; // Adjust height
    } else if (handle == SelectionHandles::TOP) { // Top
        height -= dy; // Adjust height
    } else if (handle == SelectionHandles::TOPRIGHT) { // Top-Right
        width += dx; // Adjust width
        height -= dy; // Adjust height
    } else if (handle == SelectionHandles::LEFT) { // Left
        width -= dx; // Adjust width
    } else if (handle == SelectionHandles::RIGHT) { // Right
        width += dx; // Adjust width
    } else if (handle == SelectionHandles::BOTTOMLEFT) { // Bottom-Left
        width -= dx; // Adjust width
        height += dy; // Adjust height
    } else if (handle == SelectionHandles::BOTTOM) { // Bottom
        height += dy; // Adjust height
    } else if (handle == SelectionHandles::BOTTOMRIGHT) { // Bottom-Right
        width += dx; // Adjust width
        height += dy; // Adjust height
    }
    if (width < 10) width = 10; // Min width
    if (height < 10) height = 10; // Min height
    if (handle == SelectionHandles::TOPLEFT || handle == SelectionHandles::LEFT || handle == SelectionHandles::BOTTOMLEFT) x = px-width; // Fix X
    if (handle == SelectionHandles::TOPLEFT || handle == SelectionHandles::TOP || handle == SelectionHandles::TOPRIGHT) y = py-height; // Fix Y
    double h2 = height/(double)lineCount; // Calculate height per line
    double newSize = h2/1.18; // Approx font size
    fontSize = std::max(10, (int)newSize); // Enforce min font size
}