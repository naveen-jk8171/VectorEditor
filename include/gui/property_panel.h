#pragma once

#include <QWidget>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QLineEdit>
#include <QColorDialog>
#include "model/graphics_object.h"
#include "gui/color_spectrum_widget.h"
#include "gui/canvas.h"
#include "gui/color_spectrum_widget.h"

class QLayout;
class Command;

class PropertyPanel : public QWidget {
    Q_OBJECT
public:
    explicit PropertyPanel(Canvas* canvas, QWidget* parent = nullptr);
    
    // show selected object's property
    void setTargetShape(GraphicsObject* shape);

signals:
    // signal to emit when user change a property
    void propertyChanged(GraphicsObject* shape);
    void commandGenerated(Command* command);

private:
    void addColorSection(QLayout* layout, const QString& label, bool isStroke);
    void setPanel();
    ColorSpectrumWidget* strokeSpectrum;
    ColorSpectrumWidget* fillSpectrum;
    Canvas* canvas;
    QDoubleSpinBox* strokeWidthEdit;
    int initialStrokeWidth;
    std::string initialStrokeColor, initialFillColor;
    QStackedWidget* panel;
    QWidget* noPanel;
    QWidget* rectPanel;
    QDoubleSpinBox* rxSpin;
    QDoubleSpinBox* rySpin;
    QDoubleSpinBox* rWidth;
    QDoubleSpinBox* rHeight;
    double initialRx, initialRy, initialRWidth, initialRHeight;
    QWidget* circPanel;
    QDoubleSpinBox* cr;
    double initialCircleR;
    QWidget* textPanel;
    QLineEdit* txtContent;
    QSpinBox* fontSizeEdit;
    std::string initialText;
    int initialFontSize;
    QWidget* hexPanel;
    QDoubleSpinBox* hr;
    double initialHexR;
    GraphicsObject* currentObject = nullptr;
};