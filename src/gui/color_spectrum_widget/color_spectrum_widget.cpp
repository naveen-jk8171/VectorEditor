#include "gui/color_spectrum_widget.h"

ColorSpectrumWidget::ColorSpectrumWidget(QWidget* parent) : QWidget(parent) {
    setFixedHeight(40); // fixed height
    setCursor(Qt::CrossCursor); // cursor type
    m_selectedX = width() / 2; // inital little triangle position
}