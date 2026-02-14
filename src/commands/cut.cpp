#include "commands/cut.h"

Cut::Cut(Canvas* canvas, std::vector<GraphicsObject*>& objs) : canvas(canvas), objs(objs) {}

void Cut::undo() {
    for (GraphicsObject* obj : objs) {
        canvas->addShape(obj);
    }
}

void Cut::redo() {
    for (GraphicsObject* obj : objs) {
        canvas->removeShape(obj, false);
    }
}