#include "io/xml_parser.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::string readSVGFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filepath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    file.close();
    return content;
}

void nextTag(const std::string& content, int& i) {
    while (i < content.size() && content[i] != '<') i++;
}

std::pair<std::string, std::string> getKeyValue(const std::string& content, int& i) {
    std::pair<std::string, std::string> keyValue = {"", ""};
    while (content[i] != '=') keyValue.first += content[i++];
    i += 2;
    while (content[i] != '"') keyValue.second += content[i++];
    i++;
    while (content[i] == ' ') i++;
    return keyValue;
}

std::string getTag(const std::string& content, int& i) {
    std::string tag = "";
    while (content[i] != ' ') tag += content[i++];
    while (content[i] == ' ') i++;
    return tag;
}

std::vector<GraphicsObject*> parse(const std::string& content) {
    std::vector<GraphicsObject*> shapes;
    int i = 0;
    while (i < content.size()) {
        nextTag(content, i);
        i++;
        while (i < content.size() && content[i] == '/') {
            nextTag(content, i);
            i++;
        }
        if (i >= content.size()) break;
        std::string word = getTag(content, i);
        GraphicsObject* newShape;
        if (word == "rect") {
            newShape = new Rectangle;
        } else if (word == "circle") {
            newShape = new Circle;
        } else if (word == "line") {
            newShape = new Line;
        } else if (word == "polygon") {
            newShape = new Hexagon;
        } else if (word == "text") {
            newShape = new Text;
        } else {
            continue;
        }
        while (i < content.size() && content[i] != '/' && content[i] != '>') {
            std::pair<std::string, std::string> keyValue = getKeyValue(content, i);
            newShape->setAttribute(keyValue.first, keyValue.second);
        }
        if (Text* t = dynamic_cast<Text*>(newShape)) {
            while (content[i] != '>') i++;
            i++;
            std::string textContent = "";
            while (content[i] != '<') textContent += content[i++];
            i++;
            newShape->setAttribute("content", textContent);
        }
        shapes.push_back(newShape);
    }
    return shapes;
}