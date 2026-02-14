#pragma once

#include "commands/command.h"
#include "model/graphics_object.h"
#include "gui/canvas.h"
#include <QPointF>

class Move : public Command {
public:
    Move(Canvas* canvas, std::vector<std::pair<GraphicsObject*, std::pair<QPointF, QPointF>>>& objsData);
    void undo() override;
    void redo() override;
private:
    Canvas* canvas;
    std::vector<std::pair<GraphicsObject*, std::pair<QPointF, QPointF>>> objsData;
};