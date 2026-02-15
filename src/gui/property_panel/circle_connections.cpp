#include "gui/property_panel.h"

void PropertyPanel::circleConnections() {
    connect(cr, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this] (double val) {
        if (auto* c = dynamic_cast<Circle*>(currentObject)) {
            c->r = val;
            c->width = val * 2; c->height = val * 2;
            emit propertyChanged(currentObject);
        }
    });
    connect(cr, &QDoubleSpinBox::editingFinished, this, [this] {
        if (auto* c = dynamic_cast<Circle*>(currentObject)) {
            double newval = cr->value();
            if (initialCircleR == newval) return;
            
            std::shared_ptr<GraphicsObject> obj = nullptr;
            const auto& shapes = canvas->getShapes();
            for(const auto& s : shapes) { if(s.get() == currentObject) { obj = s; break; } }
            
            if(obj) {
                auto propertyChangeCommand = std::make_unique<PropertyChange>(canvas, obj, Property::CIRCLER, initialCircleR, newval);
                propertyChangeCommand->redo();
                emit commandGenerated(propertyChangeCommand.release());
                initialCircleR = newval;
            }
        }
    });
}