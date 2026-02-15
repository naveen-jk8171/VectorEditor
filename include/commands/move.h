#pragma once
#include <memory>
#include <vector>
#include "commands/command.h"
#include "model/graphics_object.h"
#include "gui/canvas.h"
#include <QPointF>

class Move : public Command {
public:
    Move(Canvas* canvas, std::vector<std::pair<std::shared_ptr<GraphicsObject>, std::pair<QPointF, QPointF>>>& objsData);
    void undo() override; // Moves objects back to original position
    void redo() override; // Moves objects to new position
private:
    Canvas* canvas;
    std::vector<std::pair<std::shared_ptr<GraphicsObject>, std::pair<QPointF, QPointF>>> objsData; // Stores object pointer, start pos, and end pos for undo/redo
};