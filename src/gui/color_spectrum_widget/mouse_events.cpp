#include "gui/color_spectrum_widget.h"

void ColorSpectrumWidget::mousePressEvent(QMouseEvent* event) {
    handleMouse(event->pos().x(), event->pos().y());
}

void ColorSpectrumWidget::mouseMoveEvent(QMouseEvent* event) {
    handleMouse(event->pos().x(), event->pos().y());
}

void ColorSpectrumWidget::handleMouse(int x, int y) {
    m_selectedX = std::max(0, std::min(x, width()));
    if (y > height() / 2) {
        m_isGrayScale = true;
    } else {
        m_isGrayScale = false;
    }
    QColor finalColor;
    double ratio = (double)m_selectedX / width();
    if (m_isGrayScale) {
        int val = static_cast<int>(ratio * 255);
        finalColor = QColor(val, val, val);
    } else {
        int hue = static_cast<int>(ratio * 359);
        finalColor = QColor::fromHsv(hue, 255, 255);
    }
    emit colorSelected(finalColor.name());
    update();
}

void ColorSpectrumWidget::mouseReleaseEvent(QMouseEvent* event) {
    QColor finalColor;
    double ratio = (double)m_selectedX / width();
    if (m_isGrayScale) {
        int val = static_cast<int>(ratio * 255);
        finalColor = QColor(val, val, val);
    } else {
        int hue = static_cast<int>(ratio * 359);
        finalColor = QColor::fromHsv(hue, 255, 255);
    }
    emit colorFinal(finalColor.name());
}