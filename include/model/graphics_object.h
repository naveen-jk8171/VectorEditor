#pragma once

#include <string>
#include <memory>
#include "gui/selection_handle.h"
#include <math.h>
#include <QPointF>

class QGraphicsScene;
class QGraphicsItem;

class GraphicsObject{
public:
    double width = 0, height = 0, stroke_width = 2;
    std::string stroke_color = "black", fill_color = "transparent";
    virtual ~GraphicsObject() = default;
    virtual void setAttribute(const std::string& key, const std::string& value) = 0;
    virtual QGraphicsItem* draw(QGraphicsScene* scene) = 0;
    virtual std::string toSVG() const = 0;
    virtual GraphicsObject* clone() const = 0;
    virtual void move(const QPointF& pos) = 0;
    virtual QPointF getPosition() const = 0;
    virtual void resizeShape(const QPointF& currPos, const QPointF& lastPos, SelectionHandles::HandlePosition handle) = 0;
};