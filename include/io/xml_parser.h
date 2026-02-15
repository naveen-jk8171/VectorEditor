#pragma once

#include <string>
#include <vector>
#include <memory>
#include "model/graphics_object.h"
#include "model/rect.h"
#include "model/circle.h"
#include "model/line.h"
#include "model/hexagon.h"
#include "model/text.h"
#include "model/freehand.h"

std::string readSVGFile(const std::string& filepath); // reads the svg file

void nextTag(const std::string& content, int& i); // get the position of next open tag ('<') in file

std::pair<std::string, std::string> getKeyValue(const std::string& content, int& i); // gets the key value pair

std::string getTag(const std::string& content, int& i); // get the tag name (like rect, svg)

std::vector<std::shared_ptr<GraphicsObject>> parse(const std::string& content); // parses the whole svg file with the help of above functions