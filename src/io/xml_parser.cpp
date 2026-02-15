#include "io/xml_parser.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::string readSVGFile(const std::string& filepath) { // Reads entire SVG file into string 
    std::ifstream file(filepath); // Open file stream
    if (!file.is_open()) { // Check for errors
        std::cerr << "Error: Could not open file " << filepath << std::endl; // Log error
        return ""; // Return empty on failure
    }
    std::stringstream buffer; // Buffer to hold content
    buffer << file.rdbuf(); // Read file buffer
    std::string content = buffer.str(); // Convert to string
    file.close(); // Close file
    return content; // Return raw XML content
}

void nextTag(const std::string& content, int& i) { // Skips to start of next tag
    while (i < content.size() && content[i] != '<') i++; // Move index until '<' found
}

std::pair<std::string, std::string> getKeyValue(const std::string& content, int& i) { // Parses attribute="value"
    std::pair<std::string, std::string> keyValue = {"", ""}; // Init pair
    while (content[i] != '=') keyValue.first += content[i++]; // Read key
    i += 2; // Skip '="' characters
    while (content[i] != '"') keyValue.second += content[i++]; // Read value
    i++; // Skip closing quote
    while (content[i] == ' ') i++; // Skip trailing spaces
    return keyValue; // Return attribute pair
}

std::string getTag(const std::string& content, int& i) { // Extracts tag name
    std::string tag = ""; // Init tag string
    while (content[i] != ' ') tag += content[i++]; // Read until space
    while (content[i] == ' ') i++; // Skip spaces
    return tag; // Return tag name (e.g., "rect")
}

std::vector<std::shared_ptr<GraphicsObject>> parse(const std::string& content) { // Manual XML parser 
    std::vector<std::shared_ptr<GraphicsObject>> shapes; // List to hold parsed objects
    int i = 0; // Current index in content
    while (i < content.size()) { // Parse entire string
        nextTag(content, i); // Find next tag start
        i++; // Move past '<'
        while (i < content.size() && content[i] == '/') { // Skip closing tags
            nextTag(content, i); // Find next
            i++;
        }
        if (i >= content.size()) break; // End of file check
        std::string word = getTag(content, i); // Read tag name
        std::shared_ptr<GraphicsObject> newShape = nullptr; // Shape holder
        if (word == "rect") { // Identify Rectangle 
            newShape = std::make_shared<Rectangle>();
        } else if (word == "circle") { // Identify Circle 
            newShape = std::make_shared<Circle>();
        } else if (word == "line") { // Identify Line 
            newShape = std::make_shared<Line>();
        } else if (word == "polygon") { // Identify Hexagon/Polygon 
            newShape = std::make_shared<Hexagon>();
        } else if (word == "text") { // Identify Text 
            newShape = std::make_shared<Text>();
        } else if (word == "polyline" || word == "path") {
            newShape = std::make_shared<FreeHand>();
        } else {
            continue; // Skip unknown tags
        }
        while (i < content.size() && content[i] != '/' && content[i] != '>') { // Parse attributes
            std::pair<std::string, std::string> keyValue = getKeyValue(content, i); // Get attr=val
            newShape->setAttribute(keyValue.first, keyValue.second); // Apply attribute 
        }
        if (auto t = std::dynamic_pointer_cast<Text>(newShape)) { // Special handling for Text content
            while (content[i] != '>') i++; // Move to end of opening tag
            i++;
            std::string textContent = "";
            while (content[i] != '<') textContent += content[i++]; // Read inner text
            i++;
            newShape->setAttribute("content", textContent); // Store text content
        }
        shapes.push_back(newShape); // Add to model
    }
    return shapes; // Return populated model
}