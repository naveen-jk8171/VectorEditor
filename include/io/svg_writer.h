#pragma once

#include <string>
#include <vector>
#include <memory>
#include "model/graphics_object.h"

void saveSVGFile(const std::vector<std::shared_ptr<GraphicsObject>>& shapes, const std::string& filePath); // convert the shapes on canvas to svg strings and writes them in a file