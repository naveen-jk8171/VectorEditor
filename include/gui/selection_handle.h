#pragma once

#include <QObject>
#include <vector>
#include <QGraphicsItemGroup>
#include <memory>

class Canvas;
class GraphicsObject;
class QGraphicsRectItem;

class SelectionHandles : public QObject, public QGraphicsItemGroup {
    Q_OBJECT
public:
    enum HandlePosition {
        TOPLEFT,
        TOP,
        TOPRIGHT,
        LEFT,
        RIGHT,
        BOTTOMLEFT,
        BOTTOM,
        BOTTOMRIGHT,
        NONE
    };
    SelectionHandles(std::shared_ptr<GraphicsObject> targetShape, Canvas* canvas);
    void updateHandles();
    HandlePosition getHandleAt(const QPointF& pos);
    std::shared_ptr<GraphicsObject> targetShape;
private:
    void placeHandle(int ind, double x, double y);
    Canvas* canvas;
    std::vector<QGraphicsRectItem*> m_handles;
    const int HANDLE_SIZE = 8;
};