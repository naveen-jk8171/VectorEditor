#include "gui/canvas.h"

void Canvas::mouseMoveEvent(QMouseEvent* event) {
    if (isResizing && selectionHandles) { // If resizing mode active
        QPointF currPos = mapToScene(event->pos()); // Get mouse pos in scene coords
        GraphicsObject* rawObj = selectionHandles->targetShape.get(); // Get target object raw ptr
        std::shared_ptr<GraphicsObject> obj = nullptr;
        for(auto& s : m_shapes) { if(s.get() == rawObj) { obj = s; break; } } // Find shared ptr
        if (obj) {
            obj->resizeShape(currPos, resizeStartPos, dragHandle); // resize
            resizeStartPos = currPos; // Update start pos for next move
            updateShape(obj);
            selectionHandles->updateHandles(); // Move handles to new shape bounds
            emit shapeSelected(obj);
        }
        return;
    }
    if (current_tool != ToolType::SELECT) scene->clearSelection(); // Deselect if drawing
    if (isPanning) { // If panning mode active
        QPoint delta = event->pos() - lastPanPos; // Calculate drag distance
        lastPanPos = event->pos(); // Update last position
        horizontalScrollBar()->setValue(horizontalScrollBar()->value()-delta.x()); // Scroll Horizontal
        verticalScrollBar()->setValue(verticalScrollBar()->value()-delta.y()); // Scroll Verticla
        event->accept();
        return;
    }
    QGraphicsView::mouseMoveEvent(event);
    if (selectionHandles && !scene->selectedItems().isEmpty()) { // If selection active
        selectionHandles->updateHandles(); // handles synced
    }
    if (current_tool == ToolType::SELECT) { // If selection tool
        QGraphicsView::mouseMoveEvent(event); // view handle the selection box
        return;
    }
    QPointF current_point = mapToScene(event->pos()); // Get current mouse pos
    if (current_tool == ToolType::RECTANGLE) { // drawing rectangle
        auto r = std::dynamic_pointer_cast<Rectangle>(current_drawing_object);
        if (r) {
            double minX = std::min(start_point.x(), current_point.x()); // Top-left X
            double minY = std::min(start_point.y(), current_point.y()); // Top-left Y
            r->x = minX;
            r->y = minY;
            r->width = std::abs(start_point.x()-current_point.x()); // Calculate width
            r->height = std::abs(start_point.y()-current_point.y()); // Calculate height
        }
    } else if (current_tool == ToolType::CIRCLE) { // drawing circle
        auto c = std::dynamic_pointer_cast<Circle>(current_drawing_object);
        if (c) {
            double dx = current_point.x()-start_point.x();
            double dy = current_point.y()-start_point.y();
            c->r = std::sqrt(dx*dx + dy*dy); // Distance formula for radius
            c->width = 2*c->r; // Diameter
            c->height = c->width;
        }
    } else if (current_tool == ToolType::LINE) { // rawing line
        auto l = std::dynamic_pointer_cast<Line>(current_drawing_object);
        if (l) {
            l->x2 = current_point.x();
            l->y2 = current_point.y();
        }
    } else if (current_tool == ToolType::HEXAGON) { // drawing hexagon
        auto h = std::dynamic_pointer_cast<Hexagon>(current_drawing_object);
        if (h) {
            double dx = current_point.x()-start_point.x();
            double dy = current_point.y()-start_point.y();
            h->r = std::sqrt(dx*dx + dy*dy); // Radius
            h->width = h->r;
            h->height = h->r;
        }
    } else if (current_tool == ToolType::FREEHAND) { // freehand drawing
        auto f = std::dynamic_pointer_cast<FreeHand>(current_drawing_object);
        if (f) {
            f->points.push_back(current_point); // Add new point to path
        }
    } else if (current_tool == ToolType::TEXT) { // text
        auto t = std::dynamic_pointer_cast<Text>(current_drawing_object);
        if (t) {
            double minX = std::min(start_point.x(), current_point.x());
            double minY = std::min(start_point.y(), current_point.y());
            double height = std::abs(current_point.y() - start_point.y());
            double width = std::abs(current_point.x() - start_point.x());
            t->x = minX;
            t->y = minY;
            t->fontSize = std::max((int)(10/1.33), (int)(height/1.33)); // Estimate font size so that one of the corner stays at mouse position
            t->width = std::max(20.0, width);
        }
    }
    updateShape(current_drawing_object); // Redraw the shape
}