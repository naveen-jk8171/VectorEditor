#include "gui/color_spectrum_widget.h"

void ColorSpectrumWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    int w = width();
    int h = height();
    int barHeight = (h / 2) - 2;
    QRect topRect(0, 0, w, barHeight);
    QLinearGradient rainbow(0, 0, w, 0);
    rainbow.setColorAt(0.0, Qt::red);
    rainbow.setColorAt(0.16, Qt::yellow);
    rainbow.setColorAt(0.33, Qt::green);
    rainbow.setColorAt(0.5, Qt::cyan);
    rainbow.setColorAt(0.66, Qt::blue);
    rainbow.setColorAt(0.83, Qt::magenta);
    rainbow.setColorAt(1.0, Qt::red);
    painter.fillRect(topRect, rainbow);
    painter.setPen(QColor(100, 100, 100));
    painter.drawRect(topRect.adjusted(0,0,-1,-1));

    QRect botRect(0, h/2, w, barHeight);
    QLinearGradient gray(0, 0, w, 0);
    gray.setColorAt(0.0, Qt::black);
    gray.setColorAt(1.0, Qt::white);
    painter.fillRect(botRect, gray);
    painter.drawRect(botRect.adjusted(0,0,-1,-1));

    int arrowY = m_isGrayScale ? (h - 2) : (barHeight);
    QPainterPath path;
    path.moveTo(m_selectedX, arrowY);
    path.lineTo(m_selectedX - 5, arrowY + 5);
    path.lineTo(m_selectedX + 5, arrowY + 5);
    path.closeSubpath();

    painter.setBrush(Qt::black);
    painter.setPen(Qt::white);
    painter.drawPath(path);
}

void ColorSpectrumWidget::setColor(const QString& hexColor) {
    QColor c(hexColor);
    if (!c.isValid()) return;
    int w = width();
    if (w <= 0) return;
    if (c.red() == c.green() && c.green() == c.blue()) {
        m_isGrayScale = true;
        double ratio = c.red() / 255.0; 
        m_selectedX = static_cast<int>(ratio * w);
    } else {
        m_isGrayScale = false;
        int hue = c.hue();
        if (hue == -1) hue = 0;
        double ratio = hue / 359.0;
        m_selectedX = static_cast<int>(ratio * w);
    }
    update();
}