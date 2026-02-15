#pragma once

#include "model/graphics_object.h"
#include <QGraphicsScene>
#include <QPen>
#include <QColor>
#include <QBrush>
#include <vector>
#include <QVariant>
#include <vector>
#include <string>
#include <sstream>
#include <utility>
#include <iostream>
#include <algorithm>

class FreeHand : public GraphicsObject {
public:
    std::vector<QPointF> points; // vector of points in freehand drawing
    QGraphicsItem* draw(QGraphicsScene* scene) override;
    std::string toSVG() const override;
    GraphicsObject* clone() const override;
    void move(const QPointF& pos) override;
    QPointF getPosition() const override;
    void resizeShape(const QPointF& currPos, const QPointF& lastPos, SelectionHandles::HandlePosition handle) override;
    void setAttribute(const std::string& key, const std::string& value) override;
    void strToPoint(const std::string& pointsStr); // converts the string of points (from svg file) to real points
    QRectF getBoundingBox() const; // calculates the bounding box
};