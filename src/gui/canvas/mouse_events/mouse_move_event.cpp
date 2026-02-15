#include "gui/canvas.h"

void Canvas::mouseMoveEvent(QMouseEvent* event) {
    if (isResizing && selectionHandles) {
        QPointF currPos = mapToScene(event->pos());
        GraphicsObject* rawObj = selectionHandles->targetShape.get();
        std::shared_ptr<GraphicsObject> obj = nullptr;
        for(auto& s : m_shapes) { if(s.get() == rawObj) { obj = s; break; } }

        if (obj) {
            obj->resizeShape(currPos, resizeStartPos, dragHandle);
            resizeStartPos = currPos;
            updateShape(obj);
            selectionHandles->updateHandles();
            emit shapeSelected(obj);
        }
        return;
    }
    if (current_tool != ToolType::SELECT) scene->clearSelection();
    if (isPanning) {
        QPoint delta = event->pos() - lastPanPos;
        lastPanPos = event->pos();
        horizontalScrollBar()->setValue(horizontalScrollBar()->value()-delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value()-delta.y());
        event->accept();
        return;
    }
    QGraphicsView::mouseMoveEvent(event);
    if (selectionHandles && !scene->selectedItems().isEmpty()) {
        selectionHandles->updateHandles();
    }
    if (current_tool == ToolType::SELECT) {
        QGraphicsView::mouseMoveEvent(event);
        return;
    }
    QPointF current_point = mapToScene(event->pos());
    if (current_tool == ToolType::RECTANGLE) {
        auto r = std::dynamic_pointer_cast<Rectangle>(current_drawing_object);
        if (r) {
            double minX = std::min(start_point.x(), current_point.x());
            double minY = std::min(start_point.y(), current_point.y());
            r->x = minX;
            r->y = minY;
            r->width = std::abs(start_point.x()-current_point.x());
            r->height = std::abs(start_point.y()-current_point.y());
        }
    } else if (current_tool == ToolType::CIRCLE) {
        auto c = std::dynamic_pointer_cast<Circle>(current_drawing_object);
        if (c) {
            double dx = current_point.x()-start_point.x();
            double dy = current_point.y()-start_point.y();
            c->r = std::sqrt(dx*dx + dy*dy);
            c->width = 2*c->r;
            c->height = c->width;
        }
    } else if (current_tool == ToolType::LINE) {
        auto l = std::dynamic_pointer_cast<Line>(current_drawing_object);
        if (l) {
            l->x2 = current_point.x();
            l->y2 = current_point.y();
        }
    } else if (current_tool == ToolType::HEXAGON) {
        auto h = std::dynamic_pointer_cast<Hexagon>(current_drawing_object);
        if (h) {
            double dx = current_point.x()-start_point.x();
            double dy = current_point.y()-start_point.y();
            h->r = std::sqrt(dx*dx + dy*dy);
            h->width = h->r;
            h->height = h->r;
        }
    } else if (current_tool == ToolType::FREEHAND) {
        auto f = std::dynamic_pointer_cast<FreeHand>(current_drawing_object);
        if (f) {
            f->points.push_back(current_point);
        }
    } else if (current_tool == ToolType::TEXT) {
        auto t = std::dynamic_pointer_cast<Text>(current_drawing_object);
        if (t) {
            double minX = std::min(start_point.x(), current_point.x());
            double minY = std::min(start_point.y(), current_point.y());
            double height = std::abs(current_point.y() - start_point.y());
            double width = std::abs(current_point.x() - start_point.x());
            t->x = minX;
            t->y = minY;
            t->fontSize = std::max((int)(10/1.33), (int)(height/1.33));
            t->width = std::max(20.0, width);
        }
    }
    updateShape(current_drawing_object);
}