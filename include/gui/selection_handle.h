#pragma once

#include <QObject>
#include <vector>
#include <QGraphicsItemGroup>

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
    SelectionHandles(GraphicsObject* targetShape, Canvas* Canvas);
    // updte handle position to the target shape
    void updateHandles();
    // check if mouse is over a handle
    HandlePosition getHandleAt(const QPointF& pos);
    GraphicsObject* targetShape;
private:
    void placeHandle(int ind, double x, double y);
    Canvas* canvas;
    std::vector<QGraphicsRectItem*> m_handles;
    const int HANDLE_SIZE = 8;
};