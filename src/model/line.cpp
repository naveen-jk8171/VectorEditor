#include "model/line.h"

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