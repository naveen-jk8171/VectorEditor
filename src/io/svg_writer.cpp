#include "io/svg_writer.h"
#include <fstream>
#include <iostream>

void saveSVGFile(const std::vector<std::shared_ptr<GraphicsObject>>& shapes, const std::string& filePath) {
    std::ofstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for writing: " << filePath << std::endl;
        return;
    }
    file << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>";
    file << "<svg width=\"800\" height=\"600\" xmlns=\"http://www.w3.org/2000/svg\">\n";
    for (const auto& shape : shapes) {
        if (shape) {
            file << " " << shape->toSVG() << "\n";
        }
    }
    file << "</svg>";
    file.close();
}