#pragma once
#include <memory>
#include "commands/command.h"
#include <QVariant>
#include "model/graphics_object.h"
#include "gui/canvas.h"

enum class Property {
    RECTRX, RECTRY, RECTWIDTH, RECTHEIGHT, CIRCLER, TEXTCONTENT, FONTSIZE, HEXAGONR, STROKEWIDTH, STROKECOLOR, FILLCOLOR
};

class PropertyChange : public Command {
public:
    PropertyChange(Canvas* canvas, std::shared_ptr<GraphicsObject> obj, Property p, const QVariant& oldval, const QVariant& newval);
    void undo() override;
    void redo() override;
private:
    void setValue(const QVariant& value);
    Canvas* canvas;
    std::shared_ptr<GraphicsObject> obj;
    Property property;
    QVariant oldval;
    QVariant newval;
};