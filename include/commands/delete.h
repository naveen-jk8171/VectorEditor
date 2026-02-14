#pragma once
#include <memory>
#include <vector>
#include "commands/command.h"
#include "model/graphics_object.h"
#include "gui/canvas.h"

class Delete : public Command {
public:
    Delete(Canvas* canvas, std::vector<std::shared_ptr<GraphicsObject>>& objs);
    void undo() override;
    void redo() override;
private:
    std::vector<std::shared_ptr<GraphicsObject>> objs;
    Canvas* canvas;
};