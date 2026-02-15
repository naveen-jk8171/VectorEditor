#include "commands/property_change.h"
#include "model/rect.h"
#include "model/circle.h"
#include "model/hexagon.h"
#include "model/text.h"

PropertyChange::PropertyChange(Canvas* canvas, std::shared_ptr<GraphicsObject> obj, Property p, const QVariant& oldval, const QVariant& newval) : obj(obj), property(p), oldval(oldval), newval(newval), canvas(canvas) {}

void PropertyChange::undo() {
    setValue(oldval);
}

void PropertyChange::redo() {
    setValue(newval);
}

void PropertyChange::setValue(const QVariant& val) { // sets the properties values
    if (!obj) return;
    if (property == Property::STROKECOLOR) {
        obj->stroke_color = val.toString().toStdString();
    } else if (property == Property::STROKEWIDTH) {
        obj->stroke_width = val.toDouble();
    } else if (property == Property::FILLCOLOR) {
        obj->fill_color = val.toString().toStdString();
    } else if (auto r = std::dynamic_pointer_cast<Rectangle>(obj)) {
        if (property == Property::RECTRX) {
            r->rx = val.toDouble();
        } else if (property == Property::RECTRY) {
            r->ry = val.toDouble();
        } else if (property == Property::RECTWIDTH) {
            r->width = val.toDouble();
        } else if (property == Property::RECTHEIGHT) {
            r->height = val.toDouble();
        }
    } else if (auto c = std::dynamic_pointer_cast<Circle>(obj)) {
        if (property == Property::CIRCLER) c->r = val.toDouble();
    } else if (auto t = std::dynamic_pointer_cast<Text>(obj)) {
        if (property == Property::TEXTCONTENT) {
            t->content = val.toString().toStdString();
        } else if (property == Property::FONTSIZE) {
            t->fontSize = val.toInt();
        }
    } else if (auto h = std::dynamic_pointer_cast<Hexagon>(obj)) {
        if (property == Property::HEXAGONR) h->r = val.toDouble();
    }
    canvas->updateShape(obj);
    canvas->updateSelectionHandles();
    if (canvas->getVisualItem(obj)->isSelected()) emit canvas->shapeSelected(obj);
}