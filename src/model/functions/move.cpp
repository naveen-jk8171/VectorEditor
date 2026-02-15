#include "model/circle.h"
#include "model/freehand.h"
#include "model/hexagon.h"
#include "model/line.h"
#include "model/rect.h"
#include "model/text.h"

void Circle::move(const QPointF& pos) { // Move logic for Circle
    cx = pos.x(); // Update center X
    cy = pos.y(); // Update center Y
}

void FreeHand::move(const QPointF& pos) { // Move logic for Freehand
    if (points.empty()) return; // Check for empty
    QPointF currPos = getPosition(); // Get current top-left
    double dx = pos.x() - currPos.x(); // Calculate delta X
    double dy = pos.y() - currPos.y(); // Calculate delta Y
    for (auto& p : points) { // Shift all points
        p.rx() += dx; // Update X
        p.ry() += dy; // Update Y
    }
}

void Hexagon::move(const QPointF& pos) { // Move logic for Hexagon
    cx = pos.x(); // Update center X
    cy = pos.y(); // Update center Y
}

void Line::move(const QPointF& pos) { // Move logic for Line
    double dx = x2-x1; // Calculate width
    double dy = y2-y1; // Calculate height
    x1 = pos.x(); // Update start X
    y1 = pos.y(); // Update start Y
    x2 = x1 + dx; // Update end X
    y2 = y1 + dy; // Update end Y
}

void Rectangle::move(const QPointF& pos) { // Move logic for Rectangle
    x = pos.x(); // Update top-left X
    y = pos.y(); // Update top-left Y
}

void Text::move(const QPointF& pos) { // Move logic for Text
    x = pos.x(); // Update top-left X
    y = pos.y(); // Update top-left Y
}