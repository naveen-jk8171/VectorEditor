#include "io/svg_writer.h"
#include <fstream>
#include <iostream>

void saveSVGFile(const std::vector<std::shared_ptr<GraphicsObject>>& shapes, const std::string& filePath) {
    std::ofstream file(filePath); // Open file for writing

    if (!file.is_open()) { // Check if file opened successfully
        std::cerr << "Error: Could not open file for writing: " << filePath << std::endl; // Error handling
        return;
    }
    file << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>"; // Standard XML header 
    file << "<svg width=\"800\" height=\"600\" xmlns=\"http://www.w3.org/2000/svg\">\n"; // SVG root element 
    for (const auto& shape : shapes) { // Iterate through all shapes in the model
        if (shape) { // Check for valid pointer
            file << " " << shape->toSVG() << "\n"; // Write shape's SVG string 
        }
    }
    file << "</svg>"; // Close SVG tag
    file.close(); // Close file
}