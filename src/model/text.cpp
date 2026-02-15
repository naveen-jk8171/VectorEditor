#include "model/text.h"

void Text::setAttribute(const std::string& key, const std::string& value) {
    if (key == "x") {
        x = std::stod(value);
    } else if (key == "y") {
        y = std::stod(value);
    } else if (key == "content") {
        content = value;
    } else if (key == "font-family") {
        fontFamily = value;
    } else if (key == "font-size") {
        fontSize = std::stoi(value);
    } else if (key == "width") {
        width = std::stod(value);
    } else {
        GraphicsObject::setAttribute(key, value);
    }
}