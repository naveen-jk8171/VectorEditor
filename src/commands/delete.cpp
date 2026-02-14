#include "commands/delete.h"

Delete::Delete(Canvas* canvas, std::vector<std::shared_ptr<GraphicsObject>>& objs) : canvas(canvas), objs(objs) {}

void Delete::undo() {
    for (auto& obj : objs) canvas->addShape(obj);
}

void Delete::redo() {
    for (auto& obj : objs) canvas->removeShape(obj, false);
}