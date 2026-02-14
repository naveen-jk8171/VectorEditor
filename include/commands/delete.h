#pragma once

#include "commands/command.h"
#include "model/graphics_object.h"
#include "gui/canvas.h"

class Delete : public Command {
public:
    Delete(Canvas* canvas, std::vector<GraphicsObject*>& objs);
    void undo() override;
    void redo() override;
private:
    std::vector<GraphicsObject*> objs;
    Canvas* canvas;
};