#include "commands/delete.h"

Delete::Delete(Canvas* canvas, std::vector<GraphicsObject*>& objs) : canvas(canvas), objs(objs) {}

void Delete::undo() {
    for (GraphicsObject* obj : objs) canvas->addShape(obj);
}

void Delete::redo() {
    for (GraphicsObject* obj : objs) canvas->removeShape(obj, false);
}