#include "model/freehand.h"

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