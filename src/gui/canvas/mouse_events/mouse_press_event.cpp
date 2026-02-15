#include "gui/canvas.h"

void Canvas::mousePressEvent(QMouseEvent* event) {
    QGraphicsItem* clickedItem = itemAt(event->pos());
    QGraphicsItem* focusItem = scene->focusItem();
    if (focusItem && clickedItem == focusItem) {
        QGraphicsTextItem* ti = dynamic_cast<QGraphicsTextItem*>(focusItem);
        if (ti && (ti->textInteractionFlags() & Qt::TextEditorInteraction)) {
            QGraphicsView::mousePressEvent(event);
            return;
        }
    }
    if (focusItem) {
        QGraphicsTextItem* ti = dynamic_cast<QGraphicsTextItem*>(focusItem);
        if (ti) {
            ti->setTextInteractionFlags(Qt::NoTextInteraction);
            QTextCursor c = ti->textCursor();
            c.clearSelection();
            ti->setTextCursor(c);
            ti->clearFocus();
            QVariant data = ti->data(0);
            GraphicsObject* objPtr = static_cast<GraphicsObject*>(data.value<void*>());
            std::shared_ptr<GraphicsObject> obj = nullptr;
            for(auto& s : m_shapes) { if(s.get() == objPtr) { obj = s; break; } }
            if (auto t = std::dynamic_pointer_cast<Text>(obj)) {
                t->showBorder = false;
                updateShape(t);
                if (t->content.empty()) emptyTextItems.insert(t);
            }
        }
    }
    for (std::shared_ptr<GraphicsObject> obj : emptyTextItems) {
        Text* t = dynamic_cast<Text*>(obj.get());
        if (t->content.empty()) {
            removeShape(obj, true);
        }
    }
    emptyTextItems.clear();
    if (selectionHandles && current_tool == ToolType::SELECT) {
        QPointF pos = mapToScene(event->pos());
        SelectionHandles::HandlePosition handle = selectionHandles->getHandleAt(pos);
        if (handle != SelectionHandles::NONE) {
            GraphicsObject* rawObj = selectionHandles->targetShape.get();
            std::shared_ptr<GraphicsObject> obj = nullptr;
            for(auto& s : m_shapes) { if(s.get() == rawObj) { obj = s; break; } }
            
            if (auto t = std::dynamic_pointer_cast<Text>(obj)) {
                if (m_visuals.count(t)) {
                    QGraphicsItem* item = m_visuals[t];
                    t->width = item->boundingRect().width();
                    t->height = item->boundingRect().height();
                }
            }
            isResizing = true;
            dragHandle = handle;
            resizeStartPos = pos;
            event->accept();
            return;
        }
    }
    if (event->button() == Qt::MiddleButton) {
        isPanning = true;
        lastPanPos = event->pos();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    }
    if (current_tool == ToolType::SELECT) {
        QGraphicsView::mousePressEvent(event);
        updateSelectionHandles();
        initialItemPositions.clear();
        QGraphicsItem* clickedItem = scene->itemAt(mapToScene(event->pos()), QTransform());
        if (clickedItem) {
            for (QGraphicsItem* item : scene->selectedItems()) {
                QVariant data = item->data(0);
                GraphicsObject* objPtr = static_cast<GraphicsObject*>(data.value<void*>());
                std::shared_ptr<GraphicsObject> obj = nullptr;
                for(auto& s : m_shapes) { if(s.get() == objPtr) { obj = s; break; } }
                if(obj) initialItemPositions[obj] = obj->getPosition();
            }
        } else {
            emit shapeSelected(nullptr);
        }
        updateSelectionHandles();
        return;
    }
    if (current_tool != ToolType::SELECT) scene->clearSelection();
    start_point = mapToScene(event->pos());
    if (current_tool == ToolType::RECTANGLE) {
        auto r = std::make_shared<Rectangle>();
        r->x = start_point.x();
        r->y = start_point.y();
        current_drawing_object = r;
        addShape(r);
    } else if (current_tool == ToolType::CIRCLE) {
        auto c = std::make_shared<Circle>();
        c->cx = start_point.x();
        c->cy = start_point.y();
        current_drawing_object = c;
        addShape(c);
    } else if (current_tool == ToolType::LINE) {
        auto l = std::make_shared<Line>();
        l->x1 = start_point.x();
        l->x2 = start_point.x();
        l->y1 = start_point.y();
        l->y2 = start_point.y();
        current_drawing_object = l;
        addShape(l);
    } else if (current_tool == ToolType::HEXAGON) {
        auto h = std::make_shared<Hexagon>();
        h->cx = start_point.x();
        h->cy = start_point.y();
        current_drawing_object = h;
        addShape(h);
    } else if (current_tool == ToolType::FREEHAND) {
        auto f = std::make_shared<FreeHand>();
        f->points.emplace_back(start_point);
        current_drawing_object = f;
        addShape(f);
    } else if (current_tool == ToolType::TEXT) {
        auto t = std::make_shared<Text>();
        t->showBorder = true;
        t->x = start_point.x();
        t->y = start_point.y();
        t->fill_color = "black";
        emptyTextItems.insert(t);
        current_drawing_object = t;
        addShape(t);
    }
}