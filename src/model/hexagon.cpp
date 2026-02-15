#include "model/hexagon.h"

void Hexagon::setAttribute(const std::string& key, const std::string& value) { // Set attributes from parser
    if (key == "points") { // If points attribute
        strToPoint(value); // Parse points
    } else {
        GraphicsObject::setAttribute(key, value); // Fallback
    }
}

QPolygonF Hexagon::getPoints() const { // Calculate hexagon vertices
    QVector<QPointF> points; // Points vector
    for (int i = 0; i < 6; i++) { // Generate 6 points
        double ang = (60*i - 30)*(M_PI/180.0); // Calculate angle
        double x = cx + r*std::cos(ang); // Calculate X
        double y = cy + r*std::sin(ang); // Calculate Y
        points.append(QPointF(x, y)); // Add point
    }
    return QPolygonF(points); // Return polygon
}

void Hexagon::pointsToParam(const std::vector<std::pair<double, double>>& points) { // Infer params from points
    double sumX = 0, sumY = 0; // Init sums
    for (auto& p : points) { // Sum coords
        sumX += p.first;
        sumY += p.second;
    }
    cx = sumX/6.0; // Calculate center X
    cy = sumY/6.0; // Calculate center Y
    double dx = cx - points[0].first; // Delta X to first point
    double dy = cy - points[0].second; // Delta Y to first point
    r = std::sqrt(dx*dx + dy*dy); // Calculate radius
}

void Hexagon::strToPoint(const std::string& pointsStr) { // Parse points string
    std::vector<std::pair<double, double>> points; // Points vector
    std::stringstream ss(pointsStr); // String stream
    std::string segment; // Segment
    while (std::getline(ss, segment, ' ')) { // Split by space
        if (segment.empty()) continue; // Skip empty
        int commaPos = segment.find(','); // Find separator
        if (commaPos != std::string::npos) { // If valid
            std::string xStr = segment.substr(0, commaPos); // Get X
            std::string yStr = segment.substr(commaPos + 1); // Get Y
            double x = std::stod(xStr); // Parse X
            double y = std::stod(yStr); // Parse Y
            points.push_back({x, y}); // Add point
        }
    }
    pointsToParam(points); // Calculate params from points
}