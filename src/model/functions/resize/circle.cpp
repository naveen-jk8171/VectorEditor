#include "model/circle.h"

void Circle::resizeShape(const QPointF& currPos, const QPointF& lastPos, SelectionHandles::HandlePosition handle) { // Resize logic 
    double dx = currPos.x()-lastPos.x(); // Calculate X change
    double dy = currPos.y()-lastPos.y(); // Calculate Y change
    if (handle == SelectionHandles::TOPLEFT) { // If top-left handle
        if (currPos.x() > cx && currPos.y() > cy) { // Prevent inversion
            r = 5; // Min radius
            return;
        }
        r = std::sqrt((currPos.x()-cx)*(currPos.x()-cx)+(currPos.y()-cy)*(currPos.y()-cy))/std::sqrt(2); // Calc radius
    } else if (handle == SelectionHandles::TOP) { // Top handle
        r -= dy; // Adjust radius by Y
    } else if (handle == SelectionHandles::TOPRIGHT) { // Top-right handle
        if (currPos.x() < cx && currPos.y() > cy) { // Prevent inversion
            r = 5; // Min radius
            return;
        }
        r = std::sqrt((currPos.x()-cx)*(currPos.x()-cx)+(currPos.y()-cy)*(currPos.y()-cy))/std::sqrt(2); // Calc radius
    } else if (handle == SelectionHandles::LEFT) { // Left handle
        r -= dx; // Adjust radius by X
    } else if (handle == SelectionHandles::RIGHT) { // Right handle
        r += dx; // Adjust radius by X
    } else if (handle == SelectionHandles::BOTTOMLEFT) { // Bottom-left handle
        if (currPos.x() > cx && currPos.y() < cy) { // Prevent inversion
            r = 5; // Min radius
            return;
        }
        r = std::sqrt((currPos.x()-cx)*(currPos.x()-cx)+(currPos.y()-cy)*(currPos.y()-cy))/std::sqrt(2); // Calc radius
    } else if (handle == SelectionHandles::BOTTOM) { // Bottom handle
        r += dy; // Adjust radius by Y
    } else if (handle == SelectionHandles::BOTTOMRIGHT) { // Bottom-right handle
        if (currPos.x() < cx && currPos.y() < cy) { // Prevent inversion
            r = 5; // Min radius
            return;
        }
        r = std::sqrt((currPos.x()-cx)*(currPos.x()-cx)+(currPos.y()-cy)*(currPos.y()-cy))/std::sqrt(2); // Calc radius
    }
    if (r < 5) r = 5; // Enforce minimum size
}