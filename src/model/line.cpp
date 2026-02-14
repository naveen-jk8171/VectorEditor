#include "model/line.h"
#include <QVariant>

void Line::setAttribute(const std::string& key, const std::string& value) {
    if (key == "x1") {
        x1 = std::stod(value);
    } else if (key == "x2") {
        x2 = std::stod(value);
    } else if (key == "y1") {
        y1 = std::stod(value);
    } else if (key == "y2") {
        y2 = std::stod(value);
    } else {
        GraphicsObject::setAttribute(key, value);
    }
}

QGraphicsItem* Line::draw(QGraphicsScene* scene) {
    QGraphicsLineItem* item = new QGraphicsLineItem(0, 0, x2-x1, y2-y1);
    item->setPos(x1, y1);
    QColor sColor = stroke_color.empty()? Qt::black : QColor(QString::fromStdString(stroke_color));
    item->setPen(QPen(sColor, stroke_width));
    item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    item->setData(0, QVariant::fromValue(static_cast<void*>(this)));
    scene->addItem(item);
    return item;
}

std::string Line::toSVG() const {
    return "<line x1=\"" + std::to_string(x1) + "\" x2=\"" + std::to_string(x2) + "\" y1=\"" + std::to_string(y1) + "\" y2=\"" + std::to_string(y2) + "\" stroke=\"" + stroke_color + "\" stroke-width=\"" + std::to_string(stroke_width) + "\" />";
}

GraphicsObject* Line::clone() const {
    return new Line(*this);
}

void Line::resizeShape(const QPointF& currPos, const QPointF& lastPos, SelectionHandles::HandlePosition handle) {
    if (handle == SelectionHandles::LEFT) {
        x1 = currPos.x();
        y1 = currPos.y();
    } else if (handle == SelectionHandles::RIGHT) {
        x2 = currPos.x();
        y2 = currPos.y();
    }
}

void Line::move(const QPointF& pos) {
    double dx = x2-x1;
    double dy = y2-y1;
    x1 = pos.x();
    y1 = pos.y();
    x2 = x1 + dx;
    y2 = y1 + dy;
}

QPointF Line::getPosition() const {
    return QPointF(x1, y1);
}