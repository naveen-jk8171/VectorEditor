#pragma once
#include <memory>
#include "commands/command.h"
#include "model/graphics_object.h"
#include "gui/canvas.h"

class AddShape : public Command {
public:
    AddShape(Canvas* canvas, std::shared_ptr<GraphicsObject> obj);
    void undo() override;
    void redo() override;
private:
    Canvas* canvas;
    std::shared_ptr<GraphicsObject> obj;
};