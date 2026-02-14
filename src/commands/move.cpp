#include "commands/move.h"

Move::Move(Canvas* canvas, std::vector<std::pair<GraphicsObject*, std::pair<QPointF, QPointF>>>& objsData) : canvas(canvas), objsData(objsData) {}

void Move::undo() {
    for (auto& p : objsData) {
        p.first->move(p.second.first);
        canvas->updateShape(p.first);
    }
    canvas->updateSelectionHandles();
}

void Move::redo() {
    for (auto& p : objsData) {
        p.first->move(p.second.second);
        canvas->updateShape(p.first);
    }
    canvas->updateSelectionHandles();
}