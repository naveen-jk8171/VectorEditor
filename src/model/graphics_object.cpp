#include "model/graphics_object.h"

void GraphicsObject::setAttribute(const std::string& key, const std::string& value) {
    if (key == "width") {
        width = std::stod(value);
    } else if (key == "height") {
        height = std::stod(value);
    } else if (key == "stroke-width") {
        stroke_width = std::stod(value);
    } else if (key == "stroke") {
        stroke_color = value;
    } else if (key == "fill") {
        fill_color = value;
    }
}