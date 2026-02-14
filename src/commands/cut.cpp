#include "commands/cut.h"

Cut::Cut(Canvas* canvas, std::vector<std::shared_ptr<GraphicsObject>>& objs) : canvas(canvas), objs(objs) {}

void Cut::undo() {
    for (auto& obj : objs) {
        canvas->addShape(obj);
    }
}

void Cut::redo() {
    for (auto& obj : objs) {
        canvas->removeShape(obj, false);
    }
}