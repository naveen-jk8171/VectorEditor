#include "gui/property_panel.h"

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