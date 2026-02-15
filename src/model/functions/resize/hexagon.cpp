#include "model/hexagon.h"

void Hexagon::resizeShape(const QPointF& currPos, const QPointF& lastPos, SelectionHandles::HandlePosition handle) {
    double dx = currPos.x()-lastPos.x();
    double dy = currPos.y()-lastPos.y();
    if (handle == SelectionHandles::TOPLEFT) {
        if (currPos.x() > cx && currPos.y() > cy) {
            r = 5;
            return;
        }
        r = 2.0*(std::sqrt((currPos.x()-cx)*(currPos.x()-cx)+(currPos.y()-cy)*(currPos.y()-cy))/std::sqrt(7));
    } else if (handle == SelectionHandles::TOP) {
        r -= dy;
    } else if (handle == SelectionHandles::TOPRIGHT) {
        if (currPos.x() < cx && currPos.y() > cy) {
            r = 5;
            return;
        }
        r = 2.0*(std::sqrt((currPos.x()-cx)*(currPos.x()-cx)+(currPos.y()-cy)*(currPos.y()-cy))/std::sqrt(7));
    } else if (handle == SelectionHandles::LEFT) {
        r = 2.0*(std::sqrt((currPos.x()-cx)*(currPos.x()-cx)+(currPos.y()-cy)*(currPos.y()-cy))/std::sqrt(3));
    } else if (handle == SelectionHandles::RIGHT) {
        r = 2.0*(std::sqrt((currPos.x()-cx)*(currPos.x()-cx)+(currPos.y()-cy)*(currPos.y()-cy))/std::sqrt(3));
    } else if (handle == SelectionHandles::BOTTOMLEFT) {
        if (currPos.x() > cx && currPos.y() < cy) {
            r = 5;
            return;
        }
        r = 2.0*(std::sqrt((currPos.x()-cx)*(currPos.x()-cx)+(currPos.y()-cy)*(currPos.y()-cy))/std::sqrt(7));
    } else if (handle == SelectionHandles::BOTTOM) {
        r += dy;
    } else if (handle == SelectionHandles::BOTTOMRIGHT) {
        if (currPos.x() < cx && currPos.y() < cy) {
            r = 5;
            return;
        }
        r = 2.0*(std::sqrt((currPos.x()-cx)*(currPos.x()-cx)+(currPos.y()-cy)*(currPos.y()-cy))/std::sqrt(7));
    }
    if (r < 5) r = 5;
}