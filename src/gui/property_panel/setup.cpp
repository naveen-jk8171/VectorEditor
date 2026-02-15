#include "gui/property_panel.h"

void PropertyPanel::setPanel() { // Initialize dynamic panel sections 
    noPanel = new QWidget(); // Empty panel
    panel->addWidget(noPanel);

    rectPanel = new QWidget(); // Rectangle properties
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

    circPanel = new QWidget(); // Circle properties
    QFormLayout* circlLayout = new QFormLayout(circPanel);
    circlLayout->setLabelAlignment(Qt::AlignRight);
    cr = new QDoubleSpinBox();
    cr->setRange(0, INT_MAX);
    circlLayout->addRow("Radius: ", cr);
    panel->addWidget(circPanel);

    textPanel = new QWidget(); // Text properties
    QFormLayout* textLayout = new QFormLayout(textPanel);
    textLayout->setLabelAlignment(Qt::AlignRight);
    txtContent = new QLineEdit();
    fontSizeEdit = new QSpinBox();
    fontSizeEdit->setRange(10, 500);
    textLayout->addRow("Content: ", txtContent);
    textLayout->addRow("Font Size: ", fontSizeEdit);
    panel->addWidget(textPanel);

    hexPanel = new QWidget(); // Hexagon properties
    QFormLayout* hexLayout = new QFormLayout(hexPanel);
    hexLayout->setLabelAlignment(Qt::AlignRight);
    hr = new QDoubleSpinBox();
    hr->setRange(0, INT_MAX);
    hexLayout->addRow("Radius: ", hr);
    panel->addWidget(hexPanel);

    rectangleConnections(); // Connect signals
    circleConnections();
    textConnections();
    hexConnections();
}