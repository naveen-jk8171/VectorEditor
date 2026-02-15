#include "gui/property_panel.h"

void PropertyPanel::setTargetShape(GraphicsObject* shape) { // Update panel for selected shape 
    currentObject = shape; // Set current object
    if (!shape) { // If no shape selected
        panel->setCurrentWidget(noPanel); // Show empty panel
        this->strokeWidthEdit->setValue(0); // Reset stroke width
        this->setEnabled(false); // Disable panel
        return;
    }

    this->setEnabled(true); // Enable panel
    initialStrokeWidth = shape->stroke_width; // Store initial properties for undo
    initialStrokeColor = shape->stroke_color;
    initialFillColor = shape->fill_color;
    strokeWidthEdit->setValue(shape->stroke_width); // Update UI values
    strokeSpectrum->setColor(QString::fromStdString(shape->stroke_color));
    fillSpectrum->setColor(QString::fromStdString(shape->fill_color));
    if (auto* r = dynamic_cast<Rectangle*>(currentObject)) { // If Rectangle
        initialRHeight = r->height; // Store initial dimensions
        initialRWidth = r->width;
        initialRx = r->rx;
        initialRy = r->ry;
        rxSpin->setValue(r->rx); // Update UI
        rySpin->setValue(r->ry);
        rWidth->setValue(r->width);
        rHeight->setValue(r->height);
        panel->setCurrentWidget(rectPanel); // Show rectangle panel
    } else if (auto* c = dynamic_cast<Circle*>(currentObject)) { // If Circle
        initialCircleR = c->r; // Store initial radius
        cr->setValue(c->r); // Update UI
        panel->setCurrentWidget(circPanel); // Show circle panel
    } else if (auto* t = dynamic_cast<Text*>(currentObject)) { // If Text
        initialText = t->content; // Store initial content
        initialFontSize = t->fontSize;
        txtContent->setText(QString::fromStdString(t->content)); // Update UI
        fontSizeEdit->setValue(t->fontSize);
        panel->setCurrentWidget(textPanel); // Show text panel
    } else if (auto* h = dynamic_cast<Hexagon*>(currentObject)) { // If Hexagon
        initialHexR = h->r; // Store initial radius
        hr->setValue(h->r); // Update UI
        panel->setCurrentWidget(hexPanel); // Show hexagon panel
    } else { // Other shapes (Line, Freehand)
        panel->setCurrentWidget(noPanel); // Show generic panel
    }
}