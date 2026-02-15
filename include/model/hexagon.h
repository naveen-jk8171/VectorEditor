#pragma once

#include "model/graphics_object.h"
#include <QGraphicsScene>
#include <QPen>
#include <QColor>
#include <QBrush>
#include <QGraphicsPolygonItem>
#include <QVariant>
#include <vector>
#include <string>
#include <sstream>
#include <utility>
#include <iostream>

class Hexagon : public GraphicsObject {
public:
    double cx, cy, r = 0;
    void setAttribute(const std::string& key, const std::string& value) override;
    QGraphicsItem* draw(QGraphicsScene* scene) override;
    std::string toSVG() const override;
    GraphicsObject* clone() const override;
    void resizeShape(const QPointF& currPos, const QPointF& lastPos, SelectionHandles::HandlePosition handle) override;
    void move(const QPointF& pos) override;
    QPointF getPosition() const override;
    QPolygonF getPoints() const;
    void pointsToParam(const std::vector<std::pair<double, double>>& points);
    void strToPoint(const std::string& pointsStr);
};