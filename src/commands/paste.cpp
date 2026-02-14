#include "commands/paste.h"

Paste::Paste(Canvas* canvas, std::vector<std::shared_ptr<GraphicsObject>>& objs) : canvas(canvas), objs(objs) {}

Paste::~Paste() {
    objs.clear();
}

void Paste::undo() {
    for (auto& obj : objs) canvas->removeShape(obj, false);
}

void Paste::redo() {
    for (auto& obj : objs) canvas->addShape(obj);
}