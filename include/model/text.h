#pragma once

#include "model/graphics_object.h"
#include <QGraphicsScene>
#include <QFont>

class Canvas;

class Text : public GraphicsObject {
public:
    double x = 0, y = 0, width = -1;
    std::string content = "";
    std::string fontFamily = "Arial";
    int fontSize = 20;
    bool showBorder = false;
    void setAttribute(const std::string& key, const std::string& value) override;
    QGraphicsItem* draw(QGraphicsScene* scene) override;
    std::string toSVG() const override;
    GraphicsObject* clone() const override;
    void move(const QPointF& pos) override;
    QPointF getPosition() const override;
    Canvas* canvas;
    void resizeShape(const QPointF& currPos, const QPointF& lastPos, SelectionHandles::HandlePosition handle) override;
    QGraphicsTextItem* visualItem;
};