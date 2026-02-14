#include "model/circle.h"
#include <QVariant>

void Circle::setAttribute(const std::string& key, const std::string& value) {
    if (key == "cx") {
        cx = std::stod(value);
    } else if (key == "cy") {
        cy = std::stod(value);
    } else if (key == "r") {
        r = std::stod(value);
    } else {
        GraphicsObject::setAttribute(key, value);
    }
}

QGraphicsItem* Circle::draw(QGraphicsScene* scene) {
    QGraphicsEllipseItem* item = new QGraphicsEllipseItem(-r, -r, 2*r, 2*r);
    item->setPos(cx, cy);
    QColor sColor = stroke_color.empty()? Qt::black : QColor(QString::fromStdString(stroke_color));
    item->setPen(QPen(sColor, stroke_width));
    if (!fill_color.empty()) {
        item->setBrush(QBrush(QColor(QString::fromStdString(fill_color))));
    }
    item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    item->setData(0, QVariant::fromValue(static_cast<void*>(this)));
    scene->addItem(item);
    return item;
}

std::string Circle::toSVG() const {
    return "<circle cx=\"" + std::to_string(cx) + "\" cy=\"" + std::to_string(cy) + "\" r=\"" + std::to_string(r) + "\" stroke=\"" + stroke_color + "\" fill=\"" + fill_color + "\" stroke-width=\"" + std::to_string(stroke_width) + "\" />";
}

GraphicsObject* Circle::clone() const {
    return new Circle(*this);
}

void Circle::resizeShape(const QPointF& currPos, const QPointF& lastPos, SelectionHandles::HandlePosition handle) {
    double dx = currPos.x()-lastPos.x();
    double dy = currPos.y()-lastPos.y();
    if (handle == SelectionHandles::TOPLEFT) {
        if (currPos.x() > cx && currPos.y() > cy) {
            r = 5;
            return;
        }
        r = std::sqrt((currPos.x()-cx)*(currPos.x()-cx)+(currPos.y()-cy)*(currPos.y()-cy))/std::sqrt(2);
    } else if (handle == SelectionHandles::TOP) {
        r -= dy;
    } else if (handle == SelectionHandles::TOPRIGHT) {
        if (currPos.x() < cx && currPos.y() > cy) {
            r = 5;
            return;
        }
        r = std::sqrt((currPos.x()-cx)*(currPos.x()-cx)+(currPos.y()-cy)*(currPos.y()-cy))/std::sqrt(2);
    } else if (handle == SelectionHandles::LEFT) {
        r -= dx;
    } else if (handle == SelectionHandles::RIGHT) {
        r += dx;
    } else if (handle == SelectionHandles::BOTTOMLEFT) {
        if (currPos.x() > cx && currPos.y() < cy) {
            r = 5;
            return;
        }
        r = std::sqrt((currPos.x()-cx)*(currPos.x()-cx)+(currPos.y()-cy)*(currPos.y()-cy))/std::sqrt(2);
    } else if (handle == SelectionHandles::BOTTOM) {
        r += dy;
    } else if (handle == SelectionHandles::BOTTOMRIGHT) {
        if (currPos.x() < cx && currPos.y() < cy) {
            r = 5;
            return;
        }
        r = std::sqrt((currPos.x()-cx)*(currPos.x()-cx)+(currPos.y()-cy)*(currPos.y()-cy))/std::sqrt(2);
    }
    if (r < 5) r = 5;
}

void Circle::move(const QPointF& pos) {
    cx = pos.x();
    cy = pos.y();
}

QPointF Circle::getPosition() const {
    return QPointF(cx, cy);
}