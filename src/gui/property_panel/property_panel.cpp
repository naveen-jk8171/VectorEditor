#include "gui/property_panel.h"

PropertyPanel::PropertyPanel(Canvas* canvas, QWidget* parent) : QWidget(parent), canvas(canvas) {
    this->setMinimumWidth(220); // minimum width for the panel
    QVBoxLayout* mainLayout = new QVBoxLayout(this); // main vertical layout
    mainLayout->setAlignment(Qt::AlignTop); // Align content to the top
    QHBoxLayout* widthLayout = new QHBoxLayout(); // horizontal layout for stroke width
    strokeWidthEdit = new QDoubleSpinBox(); 
    strokeWidthEdit->setRange(0, 200); // Range for stroke width
    widthLayout->addWidget(new QLabel("Stroke Width: ")); // Label
    widthLayout->addWidget(strokeWidthEdit); // Add spin box
    mainLayout->addLayout(widthLayout);
    connect(strokeWidthEdit, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this] (double val) {
        if (currentObject) { // If an object is selected
            currentObject->stroke_width = val; // Update stroke width
            emit propertyChanged(currentObject); // Emit signal
        }
    });
    connect(strokeWidthEdit, &QDoubleSpinBox::editingFinished, this, [this, canvas] {
        if (currentObject) { // If an object is selected
            double newval = strokeWidthEdit->value(); // Get new value
            if (initialStrokeWidth == newval) return;
            // Find shared_ptr
            std::shared_ptr<GraphicsObject> obj = nullptr;
            const auto& shapes = canvas->getShapes(); // Get shapes from canvas
            for(const auto& s : shapes) { if(s.get() == currentObject) { obj = s; break; } } // Find the object
            
            if(obj) { // If object found
                 auto propertyChangeCommand = std::make_unique<PropertyChange>(canvas, obj, Property::STROKEWIDTH, initialStrokeWidth, newval); // Create command
                 propertyChangeCommand->redo(); // Execute command
                 emit commandGenerated(propertyChangeCommand.release());
                 initialStrokeWidth = newval; // Update initial value
            }
        }
    });
    panel = new QStackedWidget(); // Create stacked widget for dynamic content
    setPanel();
    mainLayout->addWidget(panel);
    addColorSection(mainLayout, "Stroke Color: ", true); // Add stroke color section 
    addColorSection(mainLayout, "Fill Color: ", false); // Add fill color section 
    mainLayout->addStretch(); // Add stretch to push content up
    this->setEnabled(false); // Disable panel initially
}