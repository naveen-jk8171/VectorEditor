#pragma once

#include "gui/canvas.h"
#include "model/graphics_object.h"

class Paste : public Command {
public:
    Paste(Canvas* canvas, std::vector<GraphicsObject*>& objs);
    ~Paste();
    void undo() override;
    void redo() override;
private:
    std::vector<GraphicsObject*> objs;
    Canvas* canvas;
};