#include "gui/selection_handle.h"
#include "gui/canvas.h"
#include "model/line.h"
#include "model/freehand.h"
#include <QGraphicsScene>

SelectionHandles::SelectionHandles(GraphicsObject* targetShape, Canvas* canvas) : targetShape(targetShape), canvas(canvas) {
    for (int i = 0; i < 8; i++) {
        QGraphicsRectItem* handle = new QGraphicsRectItem(-HANDLE_SIZE/2, -HANDLE_SIZE/2, HANDLE_SIZE, HANDLE_SIZE); // -handlesize/2 so that selectionhandles group itself has pos(0, 0), so center point is (0, 0)
        handle->setBrush(Qt::white);
        QPen p(Qt::black);
        p.setWidth(0); 
        handle->setPen(p);
        handle->setFlag(QGraphicsItem::ItemIgnoresTransformations);
        handle->setZValue(INT_MAX);
        setZValue(INT_MAX); // set the z value of whole group
        addToGroup(handle);
        m_handles.push_back(handle);
    }
    setHandlesChildEvents(false);
    updateHandles();
}

void SelectionHandles::updateHandles() {
    QGraphicsItem* targetItem = canvas->getVisualItem(targetShape);
    if (!targetItem) return;
    QRectF b = targetItem->boundingRect();
    QPointF topLeft = targetItem->mapToScene(b.topLeft());
    QPointF bottomRight = targetItem->mapToScene(b.bottomRight());
    double x = topLeft.x();
    double y = topLeft.y();
    double w = bottomRight.x()-topLeft.x();
    double h = bottomRight.y() - topLeft.y();
    if (auto* l = dynamic_cast<Line*>(targetShape)) {
        QGraphicsLineItem* visualLine = dynamic_cast<QGraphicsLineItem*>(targetItem);
        if (visualLine) {
            QLineF localLine = visualLine->line();
            QPointF p1 = visualLine->mapToScene(localLine.p1());
            QPointF p2 = visualLine->mapToScene(localLine.p2());
            placeHandle(LEFT, p1.x(), p1.y());
            placeHandle(RIGHT, p2.x(), p2.y());
            for (int i = 0; i < 8; i++) m_handles[i]->setVisible(false);
            m_handles[LEFT]->setVisible(true);
            m_handles[RIGHT]->setVisible(true);
        }
        return;
    } else if (auto* f = dynamic_cast<FreeHand*>(targetShape)) {
        return;
    }
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
    // Since the rect is defined as -4 to +4, setting pos to (x,y)
    // puts the center of the square exactly at (x,y).
}

SelectionHandles::HandlePosition SelectionHandles::getHandleAt(const QPointF& pos) {
    for (int i = 0; i < 8; i++) {
        QPointF localPos = m_handles[i]->mapFromScene(pos);
        if (m_handles[i]->contains(localPos)) {
            return static_cast<HandlePosition>(i);
        }
    }
    return NONE;
}