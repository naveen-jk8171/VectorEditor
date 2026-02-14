#include "commands/paste.h"

Paste::Paste(Canvas* canvas, std::vector<GraphicsObject*>& objs) : canvas(canvas), objs(objs) {}

Paste::~Paste() {
    for (GraphicsObject* obj : objs) delete obj;
    objs.clear();
}

void Paste::undo() {
    for (GraphicsObject* obj : objs) canvas->removeShape(obj, false);
}

void Paste::redo() {
    for (GraphicsObject* obj : objs) canvas->addShape(obj);
}