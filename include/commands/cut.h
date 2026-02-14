#pragma once
#include "commands/command.h"
#include "model/graphics_object.h"
#include "gui/canvas.h"

class Cut : public Command {
public:
    Cut(Canvas* canvas, std::vector<GraphicsObject*>& objs);
    void undo() override;
    void redo() override;
    std::vector<GraphicsObject*> objs;
private:
    Canvas* canvas;
};