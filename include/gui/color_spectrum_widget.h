#pragma once

#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QLinearGradient>
#include <algorithm>
#include <QPainterPath>

class ColorSpectrumWidget : public QWidget {
    Q_OBJECT
public:
    explicit ColorSpectrumWidget(QWidget* parent = nullptr);
    void setColor(const QString& hexColor); // Sets the current color

signals:
    void colorSelected(const QString& hexColor); // Signal emitted when a color is selected
    void colorFinal(const QString& color); // Signal emitted when mouse is released on color spectrum

protected:
    void paintEvent(QPaintEvent* event) override; // Paints the spectrum
    void mousePressEvent(QMouseEvent* event) override; // Handles mouse press
    void mouseMoveEvent(QMouseEvent* event) override; // Handles mouse move
    void mouseReleaseEvent(QMouseEvent* event) override; // Handles mouse release

private:
    void handleMouse(int x, int y); // Helper to handle mouse interaction
    int m_selectedX = 0; // x coordinate of selected color
    bool m_isGrayScale = false; // Flag for grayscale color
};