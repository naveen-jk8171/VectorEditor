#include "model/rect.h"

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