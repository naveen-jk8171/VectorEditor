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
    void setColor(const QString& hexColor);

signals:
    void colorSelected(const QString& hexColor);
    void colorFinal(const QString& color);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    void handleMouse(int x, int y);
    int m_selectedX = 0;
    bool m_isGrayScale = false;
};