#include "gui/property_panel.h"

void PropertyPanel::addColorSection(QLayout* layout, const QString& label, bool isStroke) { // Add color selection UI 
    layout->addWidget(new QLabel(label)); // Add label
    QWidget* topRow = new QWidget(); // Create container for color buttons
    QHBoxLayout* topLayout = new QHBoxLayout(topRow); // Create layout for buttons
    topLayout->setContentsMargins(0, 0, 0, 0); // Remove margins
    topLayout->setSpacing(4); // Set spacing
    std::vector<QString> basics = {"transparent"}; // Define basic colors
    for (const QString& color : basics) { // Iterate through basic colors
        QPushButton* btn = new QPushButton(); // Create button
        btn->setFixedSize(20, 20); // Set button size
        btn->setCursor(Qt::PointingHandCursor); // Set cursor
        if (color == "transparent") { // Special styling for transparent
            btn->setStyleSheet("background-color: transparent; border: 1px dashed #555;");
            btn->setToolTip("Transparent (No Fill)");
        } else { // Styling for other colors
            btn->setStyleSheet(QString("background-color: %1; border: 1px solid #888;").arg(color));
        }
        connect(btn, &QPushButton::clicked, this, [this, isStroke, color]() { // Connect click event
            if (!currentObject) return; // If no object selected, return
            std::string oldVal = isStroke ? initialStrokeColor : initialFillColor; // Get old value
            std::string newVal = color.toStdString(); // Get new value
            if (oldVal == newVal) return; // If unchanged, return
            Property type = isStroke ? Property::STROKECOLOR : Property::FILLCOLOR; // Determine property type
            
            std::shared_ptr<GraphicsObject> obj = nullptr; // Find shared pointer
            const auto& shapes = canvas->getShapes();
            for(const auto& s : shapes) { if(s.get() == currentObject) { obj = s; break; } }
            
            if(obj) { // If object found
                auto cmd = std::make_unique<PropertyChange>(canvas, obj, type, QString::fromStdString(oldVal), QString::fromStdString(newVal)); // Create command
                cmd->redo(); // Execute command
                emit commandGenerated(cmd.release()); // Emit command
                emit propertyChanged(currentObject); // Emit change signal
                if (isStroke) initialStrokeColor = newVal; else initialFillColor = newVal; // Update initial value
            }
        });
        topLayout->addWidget(btn); // Add button to layout
    }
    QPushButton* customBtn = new QPushButton("..."); // Create custom color button
    customBtn->setFixedSize(20, 20); // Set size
    customBtn->setCursor(Qt::PointingHandCursor); // Set cursor
    customBtn->setToolTip("More Colors..."); // Set tooltip
    connect(customBtn, &QPushButton::clicked, this, [this, isStroke]() { // Connect click event
        if (!currentObject) return; // If no object selected, return
        std::string oldVal = isStroke ? initialStrokeColor : initialFillColor; // Get old value
        QColor currentClr(QString::fromStdString(oldVal)); // Create QColor from old value
        QColor selected = QColorDialog::getColor(currentClr, this, "Select Color", QColorDialog::ShowAlphaChannel); // Open color dialog
        if (selected.isValid()) { // If valid color selected
            std::string newVal = selected.name(QColor::HexArgb).toStdString(); // Get new hex string
            if (oldVal == newVal) return; // If unchanged, return
            
            std::shared_ptr<GraphicsObject> obj = nullptr; // Find shared pointer
            const auto& shapes = canvas->getShapes();
            for(const auto& s : shapes) { if(s.get() == currentObject) { obj = s; break; } }
            
            if(obj) { // If object found
                auto cmd = std::make_unique<PropertyChange>(canvas, obj, (isStroke ? Property::STROKECOLOR : Property::FILLCOLOR), QString::fromStdString(oldVal), QString::fromStdString(newVal)); // Create command
                cmd->redo(); // Execute command
                emit commandGenerated(cmd.release()); // Emit command
                emit propertyChanged(currentObject); // Emit change signal
                if (isStroke) initialStrokeColor = newVal; else initialFillColor = newVal; // Update initial value
            }
        }
    });
    topLayout->addWidget(customBtn); // Add custom button
    topLayout->addStretch(); // Add stretch
    layout->addWidget(topRow); // Add button row to main layout
    ColorSpectrumWidget* spectrum = new ColorSpectrumWidget(); // Create color spectrum widget
    if (isStroke) strokeSpectrum = spectrum; else fillSpectrum = spectrum; // Assign to correct pointer
    connect(spectrum, &ColorSpectrumWidget::colorSelected, this, [this, isStroke](const QString& hex) { // Connect color selection
        if (!currentObject) return; // If no object selected, return
        if (isStroke) currentObject->stroke_color = hex.toStdString(); else currentObject->fill_color = hex.toStdString(); // Update color
        emit propertyChanged(currentObject); // Emit change signal
    });
    connect(spectrum, &ColorSpectrumWidget::colorFinal, this, [this, isStroke](const QString& hex) { // Connect final color selection
        if (!currentObject) return; // If no object selected, return
        std::string oldVal = isStroke ? initialStrokeColor : initialFillColor; // Get old value
        std::string newVal = hex.toStdString(); // Get new value
        if (oldVal == newVal) return; // If unchanged, return
        
        std::shared_ptr<GraphicsObject> obj = nullptr; // Find shared pointer
        const auto& shapes = canvas->getShapes();
        for(const auto& s : shapes) { if(s.get() == currentObject) { obj = s; break; } }
        
        if(obj) { // If object found
            auto cmd = std::make_unique<PropertyChange>(canvas, obj, isStroke ? Property::STROKECOLOR : Property::FILLCOLOR, QString::fromStdString(oldVal), QString::fromStdString(newVal)); // Create command
            cmd->redo(); // Execute command
            emit commandGenerated(cmd.release()); // Emit command
            if (isStroke) initialStrokeColor = newVal; else initialFillColor = newVal; // Update initial value
        }
    });
    layout->addWidget(spectrum); // Add spectrum to layout
}