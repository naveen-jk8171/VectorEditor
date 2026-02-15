#include "model/circle.h"

void Circle::resizeShape(const QPointF& currPos, const QPointF& lastPos, SelectionHandles::HandlePosition handle) {
    double dx = currPos.x()-lastPos.x();
    double dy = currPos.y()-lastPos.y();
    if (handle == SelectionHandles::TOPLEFT) {
        if (currPos.x() > cx && currPos.y() > cy) {
            r = 5;
            return;
        }
        r = std::sqrt((currPos.x()-cx)*(currPos.x()-cx)+(currPos.y()-cy)*(currPos.y()-cy))/std::sqrt(2);
    } else if (handle == SelectionHandles::TOP) {
        r -= dy;
    } else if (handle == SelectionHandles::TOPRIGHT) {
        if (currPos.x() < cx && currPos.y() > cy) {
            r = 5;
            return;
        }
        r = std::sqrt((currPos.x()-cx)*(currPos.x()-cx)+(currPos.y()-cy)*(currPos.y()-cy))/std::sqrt(2);
    } else if (handle == SelectionHandles::LEFT) {
        r -= dx;
    } else if (handle == SelectionHandles::RIGHT) {
        r += dx;
    } else if (handle == SelectionHandles::BOTTOMLEFT) {
        if (currPos.x() > cx && currPos.y() < cy) {
            r = 5;
            return;
        }
        r = std::sqrt((currPos.x()-cx)*(currPos.x()-cx)+(currPos.y()-cy)*(currPos.y()-cy))/std::sqrt(2);
    } else if (handle == SelectionHandles::BOTTOM) {
        r += dy;
    } else if (handle == SelectionHandles::BOTTOMRIGHT) {
        if (currPos.x() < cx && currPos.y() < cy) {
            r = 5;
            return;
        }
        r = std::sqrt((currPos.x()-cx)*(currPos.x()-cx)+(currPos.y()-cy)*(currPos.y()-cy))/std::sqrt(2);
    }
    if (r < 5) r = 5;
}