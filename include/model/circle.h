#pragma once

#include "model/graphics_object.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QPen>
#include <QColor>
#include <QBrush>

class Circle : public GraphicsObject {
public:
    double cx, cy, r;
    void setAttribute(const std::string& key, const std::string& value) override;
    QGraphicsItem* draw(QGraphicsScene* scene) override;
    std::string toSVG() const override;
    GraphicsObject* clone() const override;
    void resizeShape(const QPointF& currPos, const QPointF& lastPos, SelectionHandles::HandlePosition handle) override;
    void move(const QPointF& pos) override;
    QPointF getPosition() const override;
};