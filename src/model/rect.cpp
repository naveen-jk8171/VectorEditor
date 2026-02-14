#include "model/rect.h"
#include <QVariant>

void Rectangle::setAttribute(const std::string& key, const std::string& value) {
    if (key == "x") {
        x = std::stod(value);
    } else if (key == "y") {
        y = std::stod(value);
    } else if (key == "rx") {
        rx = std::stod(value);
    } else if (key == "ry") {
        ry = std::stod(value);
    } else {
        GraphicsObject::setAttribute(key, value);
    }
}

QGraphicsItem* Rectangle::draw(QGraphicsScene* scene) {
    QPainterPath path;
    path.addRoundedRect(0, 0, width, height, rx, ry);
    QGraphicsPathItem* item = new QGraphicsPathItem(path);
    item->setPos(x, y);
    QColor sColor = stroke_color.empty()? Qt::black : QColor(QString::fromStdString(stroke_color));
    item->setPen(QPen(sColor, stroke_width));
    if (!fill_color.empty()) {
        item->setBrush(QBrush(QColor((QString::fromStdString(fill_color)))));
    }
    item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    item->setData(0, QVariant::fromValue(static_cast<void*>(this)));
    scene->addItem(item);
    return item;
}

std::string Rectangle::toSVG() const {
    return "<rect x=\"" + std::to_string(x) + "\" y=\"" + std::to_string(y) + "\" rx=\"" + std::to_string(rx) + "\" ry=\"" + std::to_string(ry) + "\" width=\"" + std::to_string(width) + "\" height=\"" + std::to_string(height) + "\" stroke=\"" + stroke_color + "\" fill=\"" + fill_color + "\" stroke-width=\"" + std::to_string(stroke_width) + "\" />";
}

GraphicsObject* Rectangle::clone() const {
    return new Rectangle(*this);
}

void Rectangle::resizeShape(const QPointF& currPos, const QPointF& lastPos, SelectionHandles::HandlePosition handle) {
    double dx = currPos.x()-lastPos.x();
    double dy = currPos.y()-lastPos.y();
    double px = x + width;
    double py = y + height;
    if (handle == SelectionHandles::TOPLEFT) {
        width -= dx;
        height -= dy;
    } else if (handle == SelectionHandles::TOP) {
        height -= dy;
    } else if (handle == SelectionHandles::TOPRIGHT) {
        width += dx;
        height -= dy;
    } else if (handle == SelectionHandles::LEFT) {
        width -= dx;
    } else if (handle == SelectionHandles::RIGHT) {
        width += dx;
    } else if (handle == SelectionHandles::BOTTOMLEFT) {
        width -= dx;
        height += dy;
    } else if (handle == SelectionHandles::BOTTOM) {
        height += dy;
    } else if (handle == SelectionHandles::BOTTOMRIGHT) {
        width += dx;
        height += dy;
    }
    if (width < 10) width = 10;
    if (height < 10) height = 10;
    if (handle == SelectionHandles::TOPLEFT || handle == SelectionHandles::LEFT || handle == SelectionHandles::BOTTOMLEFT) x = px-width;
    if (handle == SelectionHandles::TOPLEFT || handle == SelectionHandles::TOP || handle == SelectionHandles::TOPRIGHT) y = py-height;
}

void Rectangle::move(const QPointF& pos) {
    x = pos.x();
    y = pos.y();
}

QPointF Rectangle::getPosition() const {
    return QPointF(x, y);
}