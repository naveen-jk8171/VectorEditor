#include "model/circle.h"
#include "model/freehand.h"
#include "model/hexagon.h"
#include "model/line.h"
#include "model/rect.h"
#include "model/text.h"

GraphicsObject* Circle::clone() const {
    return new Circle(*this);
}

GraphicsObject* FreeHand::clone() const {
    return new FreeHand(*this);
}

GraphicsObject* Hexagon::clone() const {
    return new Hexagon(*this);
}

GraphicsObject* Line::clone() const {
    return new Line(*this);
}

GraphicsObject* Rectangle::clone() const {
    return new Rectangle(*this);
}

GraphicsObject* Text::clone() const {
    return new Text(*this);
}