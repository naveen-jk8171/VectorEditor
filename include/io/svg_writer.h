#pragma once

#include <string>
#include <vector>
#include "model/graphics_object.h"

void saveSVGFile(const std::vector<GraphicsObject*>& shapes, const std::string& filePath);