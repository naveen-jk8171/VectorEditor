#pragma once

#include <string>
#include <vector>
#include "model/graphics_object.h"
#include "model/rect.h"
#include "model/circle.h"
#include "model/line.h"
#include "model/hexagon.h"
#include "model/text.h"

std::string readSVGFile(const std::string& filepath);

void nextTag(const std::string& content, int& i);

std::pair<std::string, std::string> getKeyValue(const std::string& content, int& i);

std::string getTag(const std::string& content, int& i);

std::vector<GraphicsObject*> parse(const std::string& content);