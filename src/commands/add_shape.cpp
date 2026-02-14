#include "commands/add_shape.h"

AddShape::AddShape(Canvas* canvas, GraphicsObject* obj) : canvas(canvas), obj(obj) {}

void AddShape::undo() {
    canvas->removeShape(obj, false);
}

void AddShape::redo() {
    canvas->addShape(obj);
}