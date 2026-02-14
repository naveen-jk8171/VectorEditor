#include "model/freehand.h"
#include <QVariant>
#include <vector>
#include <string>
#include <sstream>
#include <utility>
#include <iostream>
#include <algorithm>

void FreeHand::strToPoint(const std::string& pointsStr) {
    std::stringstream ss(pointsStr);
    std::string segment;
    while (std::getline(ss, segment, ' ')) {
        if (segment.empty()) continue;
        int commaPos = segment.find(',');
        if (commaPos != std::string::npos) {
            std::string xStr = segment.substr(0, commaPos);
            std::string yStr = segment.substr(commaPos + 1);
            double x = std::stod(xStr);
            double y = std::stod(yStr);
            points.push_back(QPointF(x, y));
        }
    }
}

void FreeHand::setAttribute(const std::string& key, const std::string& value) {
    if (key == "points") {
        strToPoint(value);
    } else {
        GraphicsObject::setAttribute(key, value);
    }
}

std::string FreeHand::toSVG() const {
    std::string svgStr = "<polyline points=\"";
    for (const QPointF& p : points) {
        svgStr += std::to_string(p.x()) + "," + std::to_string(p.y()) + " ";
    }
    svgStr += "\" stroke=\"" + stroke_color + "\" fill=\"" + fill_color + "\" stroke-width=\"" + std::to_string(stroke_width) + "\" />";
    return svgStr;
}

GraphicsObject* FreeHand::clone() const {
    return new FreeHand(*this);
}

QGraphicsItem* FreeHand::draw(QGraphicsScene* scene) {
    if (points.empty()) return nullptr;
    QPainterPath path;
    path.moveTo(points[0]);
    for (int i = 1; i < points.size(); i++) path.lineTo(points[i]);
    QGraphicsPathItem* item = new QGraphicsPathItem(path);
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

void FreeHand::move(const QPointF& pos) {
    if (points.empty()) return;
    QPointF currPos = getPosition();
    double dx = pos.x() - currPos.x();
    double dy = pos.y() - currPos.y();
    for (auto& p : points) {
        p.rx() += dx;
        p.ry() += dy;
    }
}

QPointF FreeHand::getPosition() const {
    if (points.empty()) return QPointF(0, 0);
    return getBoundingBox().topLeft();
}

void FreeHand::resizeShape(const QPointF& currPos, const QPointF& lastPos, SelectionHandles::HandlePosition handle) {
    
}

QRectF FreeHand::getBoundingBox() const {
    if (points.empty()) return QRectF();
    double minX = std::numeric_limits<double>::max();
    double minY = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();
    double maxY = std::numeric_limits<double>::lowest();
    for (auto& p : points) {
        minX = std::min(p.x(), minX);
        minY = std::min(p.y(), minY);
        maxX = std::max(p.x(), maxX);
        maxY = std::max(p.y(), maxY);
    }
    return QRectF(minX, minY, maxX-minX, maxY-minY);
}