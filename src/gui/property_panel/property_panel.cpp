#include "gui/property_panel.h"

PropertyPanel::PropertyPanel(Canvas* canvas, QWidget* parent) : QWidget(parent), canvas(canvas) {
    this->setMinimumWidth(220);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignTop);
    QHBoxLayout* widthLayout = new QHBoxLayout();
    strokeWidthEdit = new QDoubleSpinBox();
    strokeWidthEdit->setRange(0, 200);
    widthLayout->addWidget(new QLabel("Stroke Width: "));
    widthLayout->addWidget(strokeWidthEdit);
    mainLayout->addLayout(widthLayout);
    connect(strokeWidthEdit, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this] (double val) {
        if (currentObject) {
            currentObject->stroke_width = val;
            emit propertyChanged(currentObject);
        }
    });
    connect(strokeWidthEdit, &QDoubleSpinBox::editingFinished, this, [this, canvas] {
        if (currentObject) {
            double newval = strokeWidthEdit->value();
            if (initialStrokeWidth == newval) return;
            // Find shared_ptr
            std::shared_ptr<GraphicsObject> obj = nullptr;
            const auto& shapes = canvas->getShapes();
            for(const auto& s : shapes) { if(s.get() == currentObject) { obj = s; break; } }
            
            if(obj) {
                 auto propertyChangeCommand = std::make_unique<PropertyChange>(canvas, obj, Property::STROKEWIDTH, initialStrokeWidth, newval);
                 propertyChangeCommand->redo();
                 emit commandGenerated(propertyChangeCommand.release());
                 initialStrokeWidth = newval;
            }
        }
    });
    panel = new QStackedWidget();
    setPanel();
    mainLayout->addWidget(panel);
    addColorSection(mainLayout, "Stroke Color: ", true);
    addColorSection(mainLayout, "Fill Color: ", false);
    mainLayout->addStretch();
    this->setEnabled(false);
}