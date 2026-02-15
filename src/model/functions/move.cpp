#include "model/circle.h"
#include "model/freehand.h"
#include "model/hexagon.h"
#include "model/line.h"
#include "model/rect.h"
#include "model/text.h"

void Circle::move(const QPointF& pos) {
    cx = pos.x();
    cy = pos.y();
}

void FreeHand::move(const QPointF& pos) {
    if (points.empty()) return;
    QPointF currPos = getPosition();
    double dx = pos.x() - currPos.x();
    double dy = pos.y() - currPos.y();
    for (auto& p : points) {
        p.rx() += dx;
        p.ry() += dy;
    }
}

void Hexagon::move(const QPointF& pos) {
    cx = pos.x();
    cy = pos.y();
}

void Line::move(const QPointF& pos) {
    double dx = x2-x1;
    double dy = y2-y1;
    x1 = pos.x();
    y1 = pos.y();
    x2 = x1 + dx;
    y2 = y1 + dy;
}

void Rectangle::move(const QPointF& pos) {
    x = pos.x();
    y = pos.y();
}

void Text::move(const QPointF& pos) {
    x = pos.x();
    y = pos.y();
}

