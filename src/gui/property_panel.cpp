#include "gui/property_panel.h"
#include <QFormLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include "model/rect.h"
#include "model/circle.h"
#include "model/text.h"
#include "model/hexagon.h"
#include "commands/property_change.h"

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

void PropertyPanel::setPanel() {
    noPanel = new QWidget();
    panel->addWidget(noPanel);

    rectPanel = new QWidget();
    QFormLayout* rectLayout = new QFormLayout(rectPanel);
    rectLayout->setLabelAlignment(Qt::AlignRight);
    rectLayout->setContentsMargins(0, 10, 0, 10);
    rxSpin = new QDoubleSpinBox();
    rySpin = new QDoubleSpinBox();
    rWidth = new QDoubleSpinBox();
    rHeight = new QDoubleSpinBox();
    rxSpin->setRange(0, 2000);
    rySpin->setRange(0, 2000);
    rHeight->setRange(0, INT_MAX);
    rWidth->setRange(0, INT_MAX);
    rxSpin->setFixedWidth(100);
    rySpin->setFixedWidth(100);
    rWidth->setFixedWidth(100);
    rHeight->setFixedWidth(100);
    rectLayout->addRow("Radius X: ", rxSpin);
    rectLayout->addRow("Radius Y: ", rySpin);
    rectLayout->addRow("Width: ", rWidth);
    rectLayout->addRow("Height: ", rHeight);
    panel->addWidget(rectPanel);
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

    circPanel = new QWidget();
    QFormLayout* circlLayout = new QFormLayout(circPanel);
    circlLayout->setLabelAlignment(Qt::AlignRight);
    cr = new QDoubleSpinBox();
    cr->setRange(0, INT_MAX);
    circlLayout->addRow("Radius: ", cr);
    panel->addWidget(circPanel);
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

    textPanel = new QWidget();
    QFormLayout* textLayout = new QFormLayout(textPanel);
    textLayout->setLabelAlignment(Qt::AlignRight);
    txtContent = new QLineEdit();
    fontSizeEdit = new QSpinBox();
    fontSizeEdit->setRange(10, 500);
    textLayout->addRow("Content: ", txtContent);
    textLayout->addRow("Font Size: ", fontSizeEdit);
    panel->addWidget(textPanel);
    connect(txtContent, &QLineEdit::textChanged, this, [this] (const QString& txt) {
        if (auto* t = dynamic_cast<Text*>(currentObject)) {
            t->content = txt.toStdString();
            emit propertyChanged(currentObject);
        }
    });
    connect(txtContent, &QLineEdit::editingFinished, this, [this] {
        if (auto* t = dynamic_cast<Text*>(currentObject)) {
            std::string newText = txtContent->text().toStdString();
            if (initialText == newText) return;
            // No easy conversion from QString to variant for property change, assuming string works
            
            std::shared_ptr<GraphicsObject> obj = nullptr;
            const auto& shapes = canvas->getShapes();
            for(const auto& s : shapes) { if(s.get() == currentObject) { obj = s; break; } }
            
            if(obj) {
                // Warning: PropertyChange might need variant or specific overloads. 
                // Assuming PropertyChange constructor handles QString or QVariant
                auto propertyChangeCommand = std::make_unique<PropertyChange>(canvas, obj, Property::TEXTCONTENT, QString::fromStdString(initialText), QString::fromStdString(newText));
                propertyChangeCommand->redo();
                emit commandGenerated(propertyChangeCommand.release());
                initialText = newText;
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

    hexPanel = new QWidget();
    QFormLayout* hexLayout = new QFormLayout(hexPanel);
    hexLayout->setLabelAlignment(Qt::AlignRight);
    hr = new QDoubleSpinBox();
    hr->setRange(0, INT_MAX);
    hexLayout->addRow("Radius: ", hr);
    panel->addWidget(hexPanel);
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

void PropertyPanel::addColorSection(QLayout* layout, const QString& label, bool isStroke) {
    layout->addWidget(new QLabel(label));
    QWidget* topRow = new QWidget();
    QHBoxLayout* topLayout = new QHBoxLayout(topRow);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setSpacing(4);
    std::vector<QString> basics = {"transparent"};
    for (const QString& color : basics) {
        QPushButton* btn = new QPushButton();
        btn->setFixedSize(20, 20);
        btn->setCursor(Qt::PointingHandCursor);
        if (color == "transparent") {
            btn->setStyleSheet("background-color: transparent; border: 1px dashed #555;");
            btn->setToolTip("Transparent (No Fill)");
        } else {
            btn->setStyleSheet(QString("background-color: %1; border: 1px solid #888;").arg(color));
        }
        connect(btn, &QPushButton::clicked, this, [this, isStroke, color]() {
            if (!currentObject) return;
            std::string oldVal = isStroke ? initialStrokeColor : initialFillColor;
            std::string newVal = color.toStdString();
            if (oldVal == newVal) return;
            Property type = isStroke ? Property::STROKECOLOR : Property::FILLCOLOR;
            
            std::shared_ptr<GraphicsObject> obj = nullptr;
            const auto& shapes = canvas->getShapes();
            for(const auto& s : shapes) { if(s.get() == currentObject) { obj = s; break; } }
            
            if(obj) {
                auto cmd = std::make_unique<PropertyChange>(canvas, obj, type, QString::fromStdString(oldVal), QString::fromStdString(newVal));
                cmd->redo();
                emit commandGenerated(cmd.release());
                emit propertyChanged(currentObject);
                if (isStroke) initialStrokeColor = newVal; else initialFillColor = newVal;
            }
        });
        topLayout->addWidget(btn);
    }
    QPushButton* customBtn = new QPushButton("...");
    customBtn->setFixedSize(20, 20);
    customBtn->setCursor(Qt::PointingHandCursor);
    customBtn->setToolTip("More Colors...");
    connect(customBtn, &QPushButton::clicked, this, [this, isStroke]() {
        if (!currentObject) return;
        std::string oldVal = isStroke ? initialStrokeColor : initialFillColor;
        QColor currentClr(QString::fromStdString(oldVal));
        QColor selected = QColorDialog::getColor(currentClr, this, "Select Color", QColorDialog::ShowAlphaChannel);
        if (selected.isValid()) {
            std::string newVal = selected.name(QColor::HexArgb).toStdString();
            if (oldVal == newVal) return;
            
            std::shared_ptr<GraphicsObject> obj = nullptr;
            const auto& shapes = canvas->getShapes();
            for(const auto& s : shapes) { if(s.get() == currentObject) { obj = s; break; } }
            
            if(obj) {
                auto cmd = std::make_unique<PropertyChange>(canvas, obj, (isStroke ? Property::STROKECOLOR : Property::FILLCOLOR), QString::fromStdString(oldVal), QString::fromStdString(newVal));
                cmd->redo();
                emit commandGenerated(cmd.release());
                emit propertyChanged(currentObject);
                if (isStroke) initialStrokeColor = newVal; else initialFillColor = newVal;
            }
        }
    });
    topLayout->addWidget(customBtn);
    topLayout->addStretch();
    layout->addWidget(topRow);
    ColorSpectrumWidget* spectrum = new ColorSpectrumWidget();
    if (isStroke) strokeSpectrum = spectrum; else fillSpectrum = spectrum;
    connect(spectrum, &ColorSpectrumWidget::colorSelected, this, [this, isStroke](const QString& hex) {
        if (!currentObject) return;
        if (isStroke) currentObject->stroke_color = hex.toStdString(); else currentObject->fill_color = hex.toStdString();
        emit propertyChanged(currentObject); 
    });
    connect(spectrum, &ColorSpectrumWidget::colorFinal, this, [this, isStroke](const QString& hex) {
        if (!currentObject) return;
        std::string oldVal = isStroke ? initialStrokeColor : initialFillColor;
        std::string newVal = hex.toStdString();
        if (oldVal == newVal) return;
        
        std::shared_ptr<GraphicsObject> obj = nullptr;
        const auto& shapes = canvas->getShapes();
        for(const auto& s : shapes) { if(s.get() == currentObject) { obj = s; break; } }
        
        if(obj) {
            auto cmd = std::make_unique<PropertyChange>(canvas, obj, isStroke ? Property::STROKECOLOR : Property::FILLCOLOR, QString::fromStdString(oldVal), QString::fromStdString(newVal));
            cmd->redo();
            emit commandGenerated(cmd.release());
            if (isStroke) initialStrokeColor = newVal; else initialFillColor = newVal;
        }
    });
    layout->addWidget(spectrum);
}

void PropertyPanel::setTargetShape(GraphicsObject* shape) {
    currentObject = shape;
    if (!shape) {
        panel->setCurrentWidget(noPanel);
        this->strokeWidthEdit->setValue(0);
        this->setEnabled(false);
        return;
    }

    this->setEnabled(true);
    initialStrokeWidth = shape->stroke_width;
    initialStrokeColor = shape->stroke_color;
    initialFillColor = shape->fill_color;
    strokeWidthEdit->setValue(shape->stroke_width);
    strokeSpectrum->setColor(QString::fromStdString(shape->stroke_color));
    fillSpectrum->setColor(QString::fromStdString(shape->fill_color));
    if (auto* r = dynamic_cast<Rectangle*>(currentObject)) {
        initialRHeight = r->height;
        initialRWidth = r->width;
        initialRx = r->rx;
        initialRy = r->ry;
        rxSpin->setValue(r->rx);
        rySpin->setValue(r->ry);
        rWidth->setValue(r->width);
        rHeight->setValue(r->height);
        panel->setCurrentWidget(rectPanel);
    } else if (auto* c = dynamic_cast<Circle*>(currentObject)) {
        initialCircleR = c->r;
        cr->setValue(c->r);
        panel->setCurrentWidget(circPanel);
    } else if (auto* t = dynamic_cast<Text*>(currentObject)) {
        initialText = t->content;
        initialFontSize = t->fontSize;
        txtContent->setText(QString::fromStdString(t->content));
        fontSizeEdit->setValue(t->fontSize);
        panel->setCurrentWidget(textPanel);
    } else if (auto* h = dynamic_cast<Hexagon*>(currentObject)) {
        initialHexR = h->r;
        hr->setValue(h->r);
        panel->setCurrentWidget(hexPanel);
    } else {
        panel->setCurrentWidget(noPanel);
    }
}