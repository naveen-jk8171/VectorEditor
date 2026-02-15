#include "model/circle.h"
#include "model/freehand.h"
#include "model/hexagon.h"
#include "model/line.h"
#include "model/rect.h"
#include "model/text.h"

QPointF Circle::getPosition() const {
    return QPointF(cx, cy);
}

QPointF FreeHand::getPosition() const {
    if (points.empty()) return QPointF(0, 0);
    return getBoundingBox().topLeft();
}

QPointF Hexagon::getPosition() const {
    return QPointF(cx, cy);
}

QPointF Line::getPosition() const {
    return QPointF(x1, y1);
}

QPointF Rectangle::getPosition() const {
    return QPointF(x, y);
}

QPointF Text::getPosition() const {
    return QPointF(x, y);
}