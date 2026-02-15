#include "gui/property_panel.h"

void PropertyPanel::setTargetShape(GraphicsObject* shape) {
    currentObject = shape;
    if (!shape) {
        panel->setCurrentWidget(noPanel);
        this->strokeWidthEdit->setValue(0);
        this->setEnabled(false);
        return;
    }

    this->setEnabled(true);
    initialStrokeWidth = shape->stroke_width;
    initialStrokeColor = shape->stroke_color;
    initialFillColor = shape->fill_color;
    strokeWidthEdit->setValue(shape->stroke_width);
    strokeSpectrum->setColor(QString::fromStdString(shape->stroke_color));
    fillSpectrum->setColor(QString::fromStdString(shape->fill_color));
    if (auto* r = dynamic_cast<Rectangle*>(currentObject)) {
        initialRHeight = r->height;
        initialRWidth = r->width;
        initialRx = r->rx;
        initialRy = r->ry;
        rxSpin->setValue(r->rx);
        rySpin->setValue(r->ry);
        rWidth->setValue(r->width);
        rHeight->setValue(r->height);
        panel->setCurrentWidget(rectPanel);
    } else if (auto* c = dynamic_cast<Circle*>(currentObject)) {
        initialCircleR = c->r;
        cr->setValue(c->r);
        panel->setCurrentWidget(circPanel);
    } else if (auto* t = dynamic_cast<Text*>(currentObject)) {
        initialText = t->content;
        initialFontSize = t->fontSize;
        txtContent->setText(QString::fromStdString(t->content));
        fontSizeEdit->setValue(t->fontSize);
        panel->setCurrentWidget(textPanel);
    } else if (auto* h = dynamic_cast<Hexagon*>(currentObject)) {
        initialHexR = h->r;
        hr->setValue(h->r);
        panel->setCurrentWidget(hexPanel);
    } else {
        panel->setCurrentWidget(noPanel);
    }
}