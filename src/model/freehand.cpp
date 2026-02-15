#include "model/freehand.h"

void FreeHand::strToPoint(const std::string& pointsStr) { // Parse points string from SVG
    std::stringstream ss(pointsStr); // Create string stream
    std::string segment; // Segment holder
    while (std::getline(ss, segment, ' ')) { // Split by space
        if (segment.empty()) continue; // Skip empty
        int commaPos = segment.find(','); // Find coordinate separator
        if (commaPos != std::string::npos) { // If valid format
            std::string xStr = segment.substr(0, commaPos); // Extract X
            std::string yStr = segment.substr(commaPos + 1); // Extract Y
            double x = std::stod(xStr); // Parse X
            double y = std::stod(yStr); // Parse Y
            points.push_back(QPointF(x, y)); // Add point
        }
    }
}

void FreeHand::setAttribute(const std::string& key, const std::string& value) { // Set attributes from parser
    if (key == "points") { // If points attribute
        strToPoint(value); // Parse points
    } else {
        GraphicsObject::setAttribute(key, value); // Fallback to base class
    }
}

void FreeHand::resizeShape(const QPointF& currPos, const QPointF& lastPos, SelectionHandles::HandlePosition handle) { // Resize implementation
    // Freehand resize not implemented yet
}

QRectF FreeHand::getBoundingBox() const { // Calculate bounding box
    if (points.empty()) return QRectF(); // Return empty if no points
    double minX = std::numeric_limits<double>::max(); // Init min X
    double minY = std::numeric_limits<double>::max(); // Init min Y
    double maxX = std::numeric_limits<double>::lowest(); // Init max X
    double maxY = std::numeric_limits<double>::lowest(); // Init max Y
    for (auto& p : points) { // Iterate points
        minX = std::min(p.x(), minX); // Update bounds
        minY = std::min(p.y(), minY);
        maxX = std::max(p.x(), maxX);
        maxY = std::max(p.y(), maxY);
    }
    return QRectF(minX, minY, maxX-minX, maxY-minY); // Return rect
}