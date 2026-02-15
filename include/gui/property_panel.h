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
#include <QFormLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include "model/rect.h"
#include "model/circle.h"
#include "model/text.h"
#include "model/hexagon.h"
#include "commands/property_change.h"

class QLayout;
class Command;

class PropertyPanel : public QWidget {
    Q_OBJECT
public:
    explicit PropertyPanel(Canvas* canvas, QWidget* parent = nullptr);
    void setTargetShape(GraphicsObject* shape); // Sets the selected object properties on panel

signals:
    void propertyChanged(GraphicsObject* shape); // Signal when property changes
    void commandGenerated(Command* command); // Signal when a command is created (for undo redo thing)

private:
    void addColorSection(QLayout* layout, const QString& label, bool isStroke); // adds color section
    void setPanel(); // Sets up the property panel
    void rectangleConnections(); // Connects rectangle properties
    void circleConnections(); // Connects circle properties
    void textConnections(); // Connects text properties
    void hexConnections(); // Connects hexagon properties
    ColorSpectrumWidget* strokeSpectrum; // Widget for stroke color
    ColorSpectrumWidget* fillSpectrum; // Widget for fill color
    Canvas* canvas; // Reference to canvas
    QDoubleSpinBox* strokeWidthEdit; // Input for stroke width
    int initialStrokeWidth; // Stores initial stroke width for undo
    std::string initialStrokeColor, initialFillColor; // Stores initial colors
    QStackedWidget* panel; // Stack for shape-specific panels
    QWidget* noPanel; // Empty panel
    QWidget* rectPanel; // Rectangle property panel
    QDoubleSpinBox* rxSpin; // Input for x-radius
    QDoubleSpinBox* rySpin; // Input for y-radius
    QDoubleSpinBox* rWidth; // Input for width
    QDoubleSpinBox* rHeight; // Input for height
    double initialRx, initialRy, initialRWidth, initialRHeight; // Initial rect values
    QWidget* circPanel; // Circle property panel
    QDoubleSpinBox* cr; // Input for circle radius
    double initialCircleR; // Initial circle radius
    QWidget* textPanel; // Text property panel
    QLineEdit* txtContent; // Input for text content
    QSpinBox* fontSizeEdit; // Input for font size
    std::string initialText; // Initial text content
    int initialFontSize; // Initial font size
    QWidget* hexPanel; // Hexagon property panel
    QDoubleSpinBox* hr; // Input for hexagon radius
    double initialHexR; // Initial hexagon radius
    GraphicsObject* currentObject = nullptr; // Currently selected object
};