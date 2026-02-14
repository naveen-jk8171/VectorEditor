#pragma once
#include <memory>
#include <vector>
#include "commands/command.h"
#include "gui/canvas.h"
#include "model/graphics_object.h"

class Paste : public Command {
public:
    Paste(Canvas* canvas, std::vector<std::shared_ptr<GraphicsObject>>& objs);
    ~Paste();
    void undo() override;
    void redo() override;
private:
    std::vector<std::shared_ptr<GraphicsObject>> objs;
    Canvas* canvas;
};