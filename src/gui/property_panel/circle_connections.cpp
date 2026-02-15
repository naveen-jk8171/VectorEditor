#include "gui/property_panel.h"

void PropertyPanel::circleConnections() { // Connect Circle UI elements
    connect(cr, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this] (double val) {
        if (auto* c = dynamic_cast<Circle*>(currentObject)) { // If object is Circle
            c->r = val; // Update radius
            c->width = val * 2; c->height = val * 2; // Update bounding box
            emit propertyChanged(currentObject); // Emit signal
        }
    });
    connect(cr, &QDoubleSpinBox::editingFinished, this, [this] {
        if (auto* c = dynamic_cast<Circle*>(currentObject)) { // If object is Circle
            double newval = cr->value(); // Get new value
            if (initialCircleR == newval) return; // If unchanged, return
            
            std::shared_ptr<GraphicsObject> obj = nullptr; // Find shared pointer
            const auto& shapes = canvas->getShapes();
            for(const auto& s : shapes) { if(s.get() == currentObject) { obj = s; break; } }
            
            if(obj) { // If found
                auto propertyChangeCommand = std::make_unique<PropertyChange>(canvas, obj, Property::CIRCLER, initialCircleR, newval); // Create command
                propertyChangeCommand->redo(); // Execute command
                emit commandGenerated(propertyChangeCommand.release()); // Emit command
                initialCircleR = newval; // Update initial value
            }
        }
    });
}