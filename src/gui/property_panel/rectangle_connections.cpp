#include "gui/property_panel.h"

void PropertyPanel::rectangleConnections() {
    connect(rxSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this] (double val) {
        if (auto* r = dynamic_cast<Rectangle*>(currentObject)) {
            r->rx = val;
            emit propertyChanged(currentObject);
        }
    });
    connect(rxSpin, &QDoubleSpinBox::editingFinished, this, [this] {
        if (auto* r = dynamic_cast<Rectangle*>(currentObject)) {
            double newval = rxSpin->value();
            if (initialRx == newval) return;
            
            std::shared_ptr<GraphicsObject> obj = nullptr;
            const auto& shapes = canvas->getShapes();
            for(const auto& s : shapes) { if(s.get() == currentObject) { obj = s; break; } }
            
            if(obj) {
                auto propertyChangeCommand = std::make_unique<PropertyChange>(canvas, obj, Property::RECTRX, initialRx, newval);
                propertyChangeCommand->redo();
                emit commandGenerated(propertyChangeCommand.release());
                initialRx = newval;
            }
        }
    });
    connect(rySpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this] (double val) {
        if (auto* r = dynamic_cast<Rectangle*>(currentObject)) {
            r->ry = val;
            emit propertyChanged(currentObject);
        }
    });
    connect(rySpin, &QDoubleSpinBox::editingFinished, this, [this] {
        if (auto* r = dynamic_cast<Rectangle*>(currentObject)) {
            double newval = rySpin->value();
            if (initialRy == newval) return;

            std::shared_ptr<GraphicsObject> obj = nullptr;
            const auto& shapes = canvas->getShapes();
            for(const auto& s : shapes) { if(s.get() == currentObject) { obj = s; break; } }
            
            if(obj) {
                auto propertyChangeCommand = std::make_unique<PropertyChange>(canvas, obj, Property::RECTRY, initialRy, newval);
                propertyChangeCommand->redo();
                emit commandGenerated(propertyChangeCommand.release());
                initialRy = newval;
            }
        }
    });
    connect(rWidth, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this] (double val) {
        if (auto* r = dynamic_cast<Rectangle*>(currentObject)) {
            r->width = val;
            emit propertyChanged(currentObject);
        }
    });
    connect(rWidth, &QDoubleSpinBox::editingFinished, this, [this] {
        if (auto* r = dynamic_cast<Rectangle*>(currentObject)) {
            double newval = rWidth->value();
            if (initialRWidth == newval) return;
            
            std::shared_ptr<GraphicsObject> obj = nullptr;
            const auto& shapes = canvas->getShapes();
            for(const auto& s : shapes) { if(s.get() == currentObject) { obj = s; break; } }
            
            if(obj) {
                auto propertyChangeCommand = std::make_unique<PropertyChange>(canvas, obj, Property::RECTWIDTH, initialRWidth, newval);
                propertyChangeCommand->redo();
                emit commandGenerated(propertyChangeCommand.release());
                initialRWidth = newval;
            }
        }
    });
    connect(rHeight, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this] (double val) {
        if (auto* r = dynamic_cast<Rectangle*>(currentObject)) {
            r->height = val;
            emit propertyChanged(currentObject);
        }
    });
    connect(rHeight, &QDoubleSpinBox::editingFinished, this, [this] {
        if (auto* r = dynamic_cast<Rectangle*>(currentObject)) {
            double newval = rHeight->value();
            if (initialRHeight == newval) return;
            
            std::shared_ptr<GraphicsObject> obj = nullptr;
            const auto& shapes = canvas->getShapes();
            for(const auto& s : shapes) { if(s.get() == currentObject) { obj = s; break; } }
            
            if(obj) {
                auto propertyChangeCommand = std::make_unique<PropertyChange>(canvas, obj, Property::RECTHEIGHT, initialRHeight, newval);
                propertyChangeCommand->redo();
                emit commandGenerated(propertyChangeCommand.release());
                initialRHeight = newval;
            }
        }
    });
}