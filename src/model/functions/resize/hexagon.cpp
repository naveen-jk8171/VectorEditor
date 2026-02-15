#include "model/hexagon.h"

void Hexagon::resizeShape(const QPointF& currPos, const QPointF& lastPos, SelectionHandles::HandlePosition handle) { // Resize logic 
    double dx = currPos.x()-lastPos.x(); // Calculate delta X
    double dy = currPos.y()-lastPos.y(); // Calculate delta Y
    if (handle == SelectionHandles::TOPLEFT) { // Top-left resize
        if (currPos.x() > cx && currPos.y() > cy) { // Check inversion
            r = 5; // Min size
            return;
        }
        r = 2.0*(std::sqrt((currPos.x()-cx)*(currPos.x()-cx)+(currPos.y()-cy)*(currPos.y()-cy))/std::sqrt(7)); // Update radius
    } else if (handle == SelectionHandles::TOP) { // Top resize
        r -= dy; // Adjust radius
    } else if (handle == SelectionHandles::TOPRIGHT) { // Top-right resize
        if (currPos.x() < cx && currPos.y() > cy) { // Check inversion
            r = 5; // Min size
            return;
        }
        r = 2.0*(std::sqrt((currPos.x()-cx)*(currPos.x()-cx)+(currPos.y()-cy)*(currPos.y()-cy))/std::sqrt(7)); // Update radius
    } else if (handle == SelectionHandles::LEFT) { // Left resize
        r = 2.0*(std::sqrt((currPos.x()-cx)*(currPos.x()-cx)+(currPos.y()-cy)*(currPos.y()-cy))/std::sqrt(3)); // Update radius
    } else if (handle == SelectionHandles::RIGHT) { // Right resize
        r = 2.0*(std::sqrt((currPos.x()-cx)*(currPos.x()-cx)+(currPos.y()-cy)*(currPos.y()-cy))/std::sqrt(3)); // Update radius
    } else if (handle == SelectionHandles::BOTTOMLEFT) { // Bottom-left resize
        if (currPos.x() > cx && currPos.y() < cy) { // Check inversion
            r = 5; // Min size
            return;
        }
        r = 2.0*(std::sqrt((currPos.x()-cx)*(currPos.x()-cx)+(currPos.y()-cy)*(currPos.y()-cy))/std::sqrt(7)); // Update radius
    } else if (handle == SelectionHandles::BOTTOM) { // Bottom resize
        r += dy; // Adjust radius
    } else if (handle == SelectionHandles::BOTTOMRIGHT) { // Bottom-right resize
        if (currPos.x() < cx && currPos.y() < cy) { // Check inversion
            r = 5; // Min size
            return;
        }
        r = 2.0*(std::sqrt((currPos.x()-cx)*(currPos.x()-cx)+(currPos.y()-cy)*(currPos.y()-cy))/std::sqrt(7)); // Update radius
    }
    if (r < 5) r = 5; // Clamp minimum radius
}