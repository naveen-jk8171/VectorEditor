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
    enum HandlePosition { // Enum for handle positions
        TOPLEFT, TOP, TOPRIGHT, LEFT, RIGHT, BOTTOMLEFT, BOTTOM, BOTTOMRIGHT, NONE
    };
    SelectionHandles(std::shared_ptr<GraphicsObject> targetShape, Canvas* canvas);
    void updateHandles(); // Updates handle positions
    HandlePosition getHandleAt(const QPointF& pos); // tells which handle was clicked
    std::shared_ptr<GraphicsObject> targetShape; // selected shape
private:
    void placeHandle(int ind, double x, double y); // Places a single handle
    Canvas* canvas;
    std::vector<QGraphicsRectItem*> m_handles; // List of handle items
    const int HANDLE_SIZE = 8; // fix size of handles
};