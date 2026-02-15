#include "gui/selection_handle.h"
#include "gui/canvas.h"
#include "model/line.h"
#include "model/freehand.h"
#include <QGraphicsScene>

SelectionHandles::SelectionHandles(std::shared_ptr<GraphicsObject> targetShape, Canvas* canvas) : targetShape(targetShape), canvas(canvas) {
    for (int i = 0; i < 8; i++) { // Create 8 handles
        QGraphicsRectItem* handle = new QGraphicsRectItem(-HANDLE_SIZE/2, -HANDLE_SIZE/2, HANDLE_SIZE, HANDLE_SIZE); 
        handle->setBrush(Qt::white);
        QPen p(Qt::black);
        p.setWidth(0); 
        handle->setPen(p);
        handle->setFlag(QGraphicsItem::ItemIgnoresTransformations); // To keep constant size irrespective of zoom
        handle->setZValue(INT_MAX); // Always on top of object
        setZValue(INT_MAX); 
        addToGroup(handle); // Group them
        m_handles.push_back(handle);
    }
    setHandlesChildEvents(false);
    updateHandles();
}

void SelectionHandles::updateHandles() { 
    QGraphicsItem* targetItem = canvas->getVisualItem(targetShape); // Get visual
    if (!targetItem) return;
    QRectF b = targetItem->boundingRect();
    QPointF topLeft = targetItem->mapToScene(b.topLeft()); 
    QPointF bottomRight = targetItem->mapToScene(b.bottomRight());
    double x = topLeft.x();
    double y = topLeft.y();
    double w = bottomRight.x()-topLeft.x();
    double h = bottomRight.y() - topLeft.y();
    if (auto l = std::dynamic_pointer_cast<Line>(targetShape)) { // handling for Line
        QGraphicsLineItem* visualLine = dynamic_cast<QGraphicsLineItem*>(targetItem);
        if (visualLine) {
            QLineF localLine = visualLine->line();
            QPointF p1 = visualLine->mapToScene(localLine.p1());
            QPointF p2 = visualLine->mapToScene(localLine.p2());
            placeHandle(LEFT, p1.x(), p1.y()); 
            placeHandle(RIGHT, p2.x(), p2.y());
            for (int i = 0; i < 8; i++) m_handles[i]->setVisible(false); // Hide other handles
            m_handles[LEFT]->setVisible(true);
            m_handles[RIGHT]->setVisible(true);
        }
        return;
    } else if (auto f = std::dynamic_pointer_cast<FreeHand>(targetShape)) { // no handles for freehand
        return;
    }
    // position all handles
    placeHandle(TOPLEFT, x, y);
    placeHandle(TOP, x+w/2.0, y);
    placeHandle(TOPRIGHT, x+w, y);
    placeHandle(LEFT, x, y+h/2.0);
    placeHandle(RIGHT, x+w, y+h/2.0);
    placeHandle(BOTTOMLEFT, x, y+h);
    placeHandle(BOTTOM, x+w/2.0, y+h);
    placeHandle(BOTTOMRIGHT, x+w, y+h);
}

void SelectionHandles::placeHandle(int ind, double x, double y) {
    m_handles[ind]->setPos(x, y);
}

SelectionHandles::HandlePosition SelectionHandles::getHandleAt(const QPointF& pos) { // Get on which handle user clicked
    for (int i = 0; i < 8; i++) {
        QPointF localPos = m_handles[i]->mapFromScene(pos);
        if (m_handles[i]->contains(localPos)) {
            return static_cast<HandlePosition>(i);
        }
    }
    return NONE; // No handle was clicked
}