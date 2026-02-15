#include "gui/property_panel.h"

void PropertyPanel::hexConnections() {
    connect(hr, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this] (double val) {
        if (auto* h = dynamic_cast<Hexagon*>(currentObject)) {
            h->r = val;
            emit propertyChanged(currentObject);
        }
    });
    connect(hr, &QDoubleSpinBox::editingFinished, this, [this] {
        if (auto* h = dynamic_cast<Hexagon*>(currentObject)) {
            double newval = hr->value();
            if (initialHexR == newval) return;
            
            std::shared_ptr<GraphicsObject> obj = nullptr;
            const auto& shapes = canvas->getShapes();
            for(const auto& s : shapes) { if(s.get() == currentObject) { obj = s; break; } }
            
            if(obj) {
                auto propertyChangeCommand = std::make_unique<PropertyChange>(canvas, obj, Property::HEXAGONR, initialHexR, newval);
                propertyChangeCommand->redo();
                emit commandGenerated(propertyChangeCommand.release());
                initialHexR = newval;
            }
        }
    });
}