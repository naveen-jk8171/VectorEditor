#include "model/hexagon.h"
#include <QVariant>
#include <vector>
#include <string>
#include <sstream>
#include <utility>
#include <iostream>

void Hexagon::setAttribute(const std::string& key, const std::string& value) {
    if (key == "points") {
        strToPoint(value);
    } else {
        GraphicsObject::setAttribute(key, value);
    }
}

QGraphicsItem* Hexagon::draw(QGraphicsScene* scene) {
    QGraphicsPolygonItem* item = new QGraphicsPolygonItem(getPoints());
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

std::string Hexagon::toSVG() const {
    std::string svgStr = "<polygon points=\"";
    QPolygonF points = getPoints();
    for (const QPointF& p : points) {
        svgStr += std::to_string(p.x()) + "," + std::to_string(p.y()) + " ";
    }
    svgStr += "\" stroke=\"" + stroke_color + "\" fill=\"" + fill_color + "\" stroke-width=\"" + std::to_string(stroke_width) + "\" />";
    return svgStr;
}

GraphicsObject* Hexagon::clone() const {
    return new Hexagon(*this);
}

void Hexagon::resizeShape(const QPointF& currPos, const QPointF& lastPos, SelectionHandles::HandlePosition handle) {
    double dx = currPos.x()-lastPos.x();
    double dy = currPos.y()-lastPos.y();
    if (handle == SelectionHandles::TOPLEFT) {
        if (currPos.x() > cx && currPos.y() > cy) {
            r = 5;
            return;
        }
        r = 2.0*(std::sqrt((currPos.x()-cx)*(currPos.x()-cx)+(currPos.y()-cy)*(currPos.y()-cy))/std::sqrt(7));
    } else if (handle == SelectionHandles::TOP) {
        r -= dy;
    } else if (handle == SelectionHandles::TOPRIGHT) {
        if (currPos.x() < cx && currPos.y() > cy) {
            r = 5;
            return;
        }
        r = 2.0*(std::sqrt((currPos.x()-cx)*(currPos.x()-cx)+(currPos.y()-cy)*(currPos.y()-cy))/std::sqrt(7));
    } else if (handle == SelectionHandles::LEFT) {
        r = 2.0*(std::sqrt((currPos.x()-cx)*(currPos.x()-cx)+(currPos.y()-cy)*(currPos.y()-cy))/std::sqrt(3));
    } else if (handle == SelectionHandles::RIGHT) {
        r = 2.0*(std::sqrt((currPos.x()-cx)*(currPos.x()-cx)+(currPos.y()-cy)*(currPos.y()-cy))/std::sqrt(3));
    } else if (handle == SelectionHandles::BOTTOMLEFT) {
        if (currPos.x() > cx && currPos.y() < cy) {
            r = 5;
            return;
        }
        r = 2.0*(std::sqrt((currPos.x()-cx)*(currPos.x()-cx)+(currPos.y()-cy)*(currPos.y()-cy))/std::sqrt(7));
    } else if (handle == SelectionHandles::BOTTOM) {
        r += dy;
    } else if (handle == SelectionHandles::BOTTOMRIGHT) {
        if (currPos.x() < cx && currPos.y() < cy) {
            r = 5;
            return;
        }
        r = 2.0*(std::sqrt((currPos.x()-cx)*(currPos.x()-cx)+(currPos.y()-cy)*(currPos.y()-cy))/std::sqrt(7));
    }
    if (r < 5) r = 5;
}

void Hexagon::move(const QPointF& pos) {
    cx = pos.x();
    cy = pos.y();
}

QPointF Hexagon::getPosition() const {
    return QPointF(cx, cy);
}

QPolygonF Hexagon::getPoints() const {
    QVector<QPointF> points;
    for (int i = 0; i < 6; i++) {
        double ang = (60*i - 30)*(M_PI/180.0);
        double x = cx + r*std::cos(ang);
        double y = cy + r*std::sin(ang);
        points.append(QPointF(x, y));
    }
    return QPolygonF(points);
}

void Hexagon::pointsToParam(const std::vector<std::pair<double, double>>& points) {
    double sumX = 0, sumY = 0;
    for (auto& p : points) {
        sumX += p.first;
        sumY += p.second;
    }
    cx = sumX/6.0;
    cy = sumY/6.0;
    double dx = cx - points[0].first;
    double dy = cy - points[0].second;
    r = std::sqrt(dx*dx + dy*dy);
}

void Hexagon::strToPoint(const std::string& pointsStr) {
    std::vector<std::pair<double, double>> points;
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
            points.push_back({x, y});
        }
    }
    pointsToParam(points);
}