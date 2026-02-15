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
    virtual void setAttribute(const std::string& key, const std::string& value) = 0; // receives key value pair and sets the value if key matches
    virtual QGraphicsItem* draw(QGraphicsScene* scene) = 0; // draws the cpp object
    virtual std::string toSVG() const = 0; // convert the cpp objects back to string
    virtual GraphicsObject* clone() const = 0; // clone the cpp object
    virtual void move(const QPointF& pos) = 0; // move the object
    virtual QPointF getPosition() const = 0; // return positioin of object on scene
    virtual void resizeShape(const QPointF& currPos, const QPointF& lastPos, SelectionHandles::HandlePosition handle) = 0; // maths for resizing shape
};