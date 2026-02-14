#pragma once

#include <QWidget>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QLineEdit>
#include <QColorDialog>
#include <memory> 
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
    
    // Change to raw pointer since panel doesn't own object, it just edits it
    void setTargetShape(GraphicsObject* shape);

signals:
    // Raw pointer signals (Qt signals/slots with smart pointers can be tricky, 
    // but passing raw pointer for identification is standard)
    void propertyChanged(GraphicsObject* shape);
    // Passing ownership of command
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
    
    // Raw pointer to current object being edited (ownership remains with Canvas/Model)
    GraphicsObject* currentObject = nullptr;
};