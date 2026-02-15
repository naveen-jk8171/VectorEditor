#include "gui/color_spectrum_widget.h"

void ColorSpectrumWidget::mousePressEvent(QMouseEvent* event) {
    handleMouse(event->pos().x(), event->pos().y());
}

void ColorSpectrumWidget::mouseMoveEvent(QMouseEvent* event) {
    handleMouse(event->pos().x(), event->pos().y());
}

void ColorSpectrumWidget::handleMouse(int x, int y) {
    m_selectedX = std::max(0, std::min(x, width())); // make x within widget bounds 
    if (y > height() / 2) { // Check if grayscale area was clicked
        m_isGrayScale = true; // Grayscale mode
    } else {
        m_isGrayScale = false; // Color mode
    }
    QColor finalColor;
    double ratio = (double)m_selectedX / width(); // calculate ratio (0.0 to 1.0)
    if (m_isGrayScale) { // grayscale mode
        int val = static_cast<int>(ratio * 255); // ratio to 0-255 intensity
        finalColor = QColor(val, val, val); // gray color
    } else {
        int hue = static_cast<int>(ratio * 359); // Ratio to 0-359 hue
        finalColor = QColor::fromHsv(hue, 255, 255); // Create color
    }
    emit colorSelected(finalColor.name()); // Signal color change 
    update(); // Update little triangle position
}

void ColorSpectrumWidget::mouseReleaseEvent(QMouseEvent* event) {
    QColor finalColor;
    double ratio = (double)m_selectedX / width();
    if (m_isGrayScale) {
        int val = static_cast<int>(ratio * 255); // Gray value
        finalColor = QColor(val, val, val); // Gray color
    } else {
        int hue = static_cast<int>(ratio * 359); // hue value
        finalColor = QColor::fromHsv(hue, 255, 255); // Create color
    }
    emit colorFinal(finalColor.name()); // Signal final color
}