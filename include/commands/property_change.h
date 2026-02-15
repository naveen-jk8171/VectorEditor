#pragma once
#include <memory>
#include "commands/command.h"
#include <QVariant>
#include "model/graphics_object.h"
#include "gui/canvas.h"

enum class Property { // Enum for different properties that can change
    RECTRX, RECTRY, RECTWIDTH, RECTHEIGHT, CIRCLER, TEXTCONTENT, FONTSIZE, HEXAGONR, STROKEWIDTH, STROKECOLOR, FILLCOLOR
};

class PropertyChange : public Command {
public:
    PropertyChange(Canvas* canvas, std::shared_ptr<GraphicsObject> obj, Property p, const QVariant& oldval, const QVariant& newval);
    void undo() override; // Reverts property to old value
    void redo() override; // Sets property to new value
private:
    void setValue(const QVariant& value); // Helper to apply value based on Property enum
    Canvas* canvas;
    std::shared_ptr<GraphicsObject> obj; // object whose property is changed
    Property property; // Which property is changing
    QVariant oldval; // old value
    QVariant newval; // new value
};