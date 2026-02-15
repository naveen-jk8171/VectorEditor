#include "model/circle.h"
#include "model/freehand.h"
#include "model/hexagon.h"
#include "model/line.h"
#include "model/rect.h"
#include "model/text.h"

std::string Circle::toSVG() const {
    return "<circle cx=\"" + std::to_string(cx) + "\" cy=\"" + std::to_string(cy) + "\" r=\"" + std::to_string(r) + "\" stroke=\"" + stroke_color + "\" fill=\"" + fill_color + "\" stroke-width=\"" + std::to_string(stroke_width) + "\" />";
}

std::string FreeHand::toSVG() const {
    std::string svgStr = "<polyline points=\"";
    for (const QPointF& p : points) {
        svgStr += std::to_string(p.x()) + "," + std::to_string(p.y()) + " ";
    }
    svgStr += "\" stroke=\"" + stroke_color + "\" fill=\"" + fill_color + "\" stroke-width=\"" + std::to_string(stroke_width) + "\" />";
    return svgStr;
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

std::string Line::toSVG() const {
    return "<line x1=\"" + std::to_string(x1) + "\" x2=\"" + std::to_string(x2) + "\" y1=\"" + std::to_string(y1) + "\" y2=\"" + std::to_string(y2) + "\" stroke=\"" + stroke_color + "\" stroke-width=\"" + std::to_string(stroke_width) + "\" />";
}

std::string Rectangle::toSVG() const {
    return "<rect x=\"" + std::to_string(x) + "\" y=\"" + std::to_string(y) + "\" rx=\"" + std::to_string(rx) + "\" ry=\"" + std::to_string(ry) + "\" width=\"" + std::to_string(width) + "\" height=\"" + std::to_string(height) + "\" stroke=\"" + stroke_color + "\" fill=\"" + fill_color + "\" stroke-width=\"" + std::to_string(stroke_width) + "\" />";
}

std::string Text::toSVG() const {
    return "<text x=\"" + std::to_string(x) + "\" y=\"" + std::to_string(y) + "\" width=\"" + std::to_string(width) + "\" font-family=\"" + fontFamily + "\"" + " font-size=\"" + std::to_string(fontSize) + "\" stroke=\"" + stroke_color + "\" stroke-width=\"" + std::to_string(stroke_width) + "\"" + " fill=\"" + (fill_color.empty() ? "black" : fill_color) + "\">"  + content  + "</text>";
}