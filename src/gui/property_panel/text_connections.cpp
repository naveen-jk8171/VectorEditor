#include "gui/property_panel.h"

void PropertyPanel::textConnections() {
    connect(txtContent, &QLineEdit::textChanged, this, [this] (const QString& txt) {
        if (auto* t = dynamic_cast<Text*>(currentObject)) { // If object is Text
            t->content = txt.toStdString(); // Update content
            emit propertyChanged(currentObject); // Emit signal
        }
    });
    connect(txtContent, &QLineEdit::editingFinished, this, [this] {
        if (auto* t = dynamic_cast<Text*>(currentObject)) { // If object is Text
            std::string newText = txtContent->text().toStdString(); // Get new text
            if (initialText == newText) return; // If unchanged, return
            std::shared_ptr<GraphicsObject> obj = nullptr; // Find shared pointer
            const auto& shapes = canvas->getShapes();
            for(const auto& s : shapes) { if(s.get() == currentObject) { obj = s; break; } }
            if(obj) { // If found
                auto propertyChangeCommand = std::make_unique<PropertyChange>(canvas, obj, Property::TEXTCONTENT, QString::fromStdString(initialText), QString::fromStdString(newText)); // Create command
                propertyChangeCommand->redo(); // Execute command
                emit commandGenerated(propertyChangeCommand.release()); // Emit command
                initialText = newText; // Update initial value
            }
        }
    });
    connect(fontSizeEdit, QOverload<int>::of(&QSpinBox::valueChanged), this, [this] (int val) {
        if (auto* t = dynamic_cast<Text*>(currentObject)) {
            t->fontSize = val;
            emit propertyChanged(currentObject);
        }
    });
    connect(fontSizeEdit, &QSpinBox::editingFinished, this, [this] {
        if (auto* t = dynamic_cast<Text*>(currentObject)) {
            int newval = fontSizeEdit->value();
            if (initialFontSize == newval) return;
            
            std::shared_ptr<GraphicsObject> obj = nullptr;
            const auto& shapes = canvas->getShapes();
            for(const auto& s : shapes) { if(s.get() == currentObject) { obj = s; break; } }
            
            if(obj) {
                auto propertyChangeCommand = std::make_unique<PropertyChange>(canvas, obj, Property::FONTSIZE, initialFontSize, newval);
                propertyChangeCommand->redo();
                emit commandGenerated(propertyChangeCommand.release());
                initialFontSize = newval;
            }
        }
    });
}