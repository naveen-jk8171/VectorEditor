#include "model/circle.h"

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