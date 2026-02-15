#include "gui/color_spectrum_widget.h"

ColorSpectrumWidget::ColorSpectrumWidget(QWidget* parent) : QWidget(parent) {
    setFixedHeight(40);
    setCursor(Qt::CrossCursor);
    m_selectedX = width() / 2;
}