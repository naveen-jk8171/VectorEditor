#include "model/hexagon.h"

void Hexagon::setAttribute(const std::string& key, const std::string& value) {
    if (key == "points") {
        strToPoint(value);
    } else {
        GraphicsObject::setAttribute(key, value);
    }
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