#pragma once
#include <memory>
#include "commands/command.h"
#include "model/graphics_object.h"
#include "gui/canvas.h"

class AddShape : public Command { // command to handle adding new shapes
public:
    AddShape(Canvas* canvas, std::shared_ptr<GraphicsObject> obj);
    void undo() override; // Removes the added shape
    void redo() override; // Re-adds the shape to the canvas
private:
    Canvas* canvas;
    std::shared_ptr<GraphicsObject> obj; // Pointer to the shape added
};