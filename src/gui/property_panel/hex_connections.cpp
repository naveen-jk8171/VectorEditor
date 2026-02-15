#include "gui/property_panel.h"

void PropertyPanel::hexConnections() { // Connect Hexagon UI elements
    connect(hr, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this] (double val) {
        if (auto* h = dynamic_cast<Hexagon*>(currentObject)) { // If object is Hexagon
            h->r = val; // Update radius
            emit propertyChanged(currentObject); // Emit signal
        }
    });
    connect(hr, &QDoubleSpinBox::editingFinished, this, [this] {
        if (auto* h = dynamic_cast<Hexagon*>(currentObject)) { // If object is Hexagon
            double newval = hr->value(); // Get new value
            if (initialHexR == newval) return; // If unchanged, return
            
            std::shared_ptr<GraphicsObject> obj = nullptr; // Find shared pointer
            const auto& shapes = canvas->getShapes();
            for(const auto& s : shapes) { if(s.get() == currentObject) { obj = s; break; } }
            
            if(obj) { // If found
                auto propertyChangeCommand = std::make_unique<PropertyChange>(canvas, obj, Property::HEXAGONR, initialHexR, newval); // Create command
                propertyChangeCommand->redo(); // Execute command
                emit commandGenerated(propertyChangeCommand.release()); // Emit command
                initialHexR = newval; // Update initial value
            }
        }
    });
}