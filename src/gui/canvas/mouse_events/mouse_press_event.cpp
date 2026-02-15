#include "gui/canvas.h"

void Canvas::mousePressEvent(QMouseEvent* event) {
    QGraphicsItem* clickedItem = itemAt(event->pos()); // Get item at click
    QGraphicsItem* focusItem = scene->focusItem(); // Get currently focused item
    if (focusItem && clickedItem == focusItem) { // If clicking on focused item
        QGraphicsTextItem* ti = dynamic_cast<QGraphicsTextItem*>(focusItem);
        if (ti && (ti->textInteractionFlags() & Qt::TextEditorInteraction)) { // text edit mode
            QGraphicsView::mousePressEvent(event); // default handling
            return;
        }
    }
    if (focusItem) { // If something has focus but we clicked elsewhere
        QGraphicsTextItem* ti = dynamic_cast<QGraphicsTextItem*>(focusItem);
        if (ti) { // If it was text
            ti->setTextInteractionFlags(Qt::NoTextInteraction); // Disable editing
            QTextCursor c = ti->textCursor();
            c.clearSelection(); // Clear text selection
            ti->setTextCursor(c);
            ti->clearFocus(); // Remove focus
            QVariant data = ti->data(0);
            GraphicsObject* objPtr = static_cast<GraphicsObject*>(data.value<void*>());
            std::shared_ptr<GraphicsObject> obj = nullptr;
            for(auto& s : m_shapes) { if(s.get() == objPtr) { obj = s; break; } }
            if (auto t = std::dynamic_pointer_cast<Text>(obj)) {
                t->showBorder = false; // Hide editing border
                updateShape(t);
                if (t->content.empty()) emptyTextItems.insert(t); // Mark empty text for deletion
            }
        }
    }
    for (std::shared_ptr<GraphicsObject> obj : emptyTextItems) { // Clean up empty text items
        Text* t = dynamic_cast<Text*>(obj.get());
        if (t->content.empty()) {
            removeShape(obj, true); // delete text item permanently
        }
    }
    emptyTextItems.clear(); // Clear empty items list
    if (selectionHandles && current_tool == ToolType::SELECT) { // If resize handles active
        QPointF pos = mapToScene(event->pos());
        SelectionHandles::HandlePosition handle = selectionHandles->getHandleAt(pos); // get the handle which we clicked
        if (handle != SelectionHandles::NONE) { // If handle clicked
            GraphicsObject* rawObj = selectionHandles->targetShape.get();
            std::shared_ptr<GraphicsObject> obj = nullptr;
            for(auto& s : m_shapes) { if(s.get() == rawObj) { obj = s; break; } }
            
            if (auto t = std::dynamic_pointer_cast<Text>(obj)) { // Update text dimensions before resize
                if (m_visuals.count(t)) {
                    QGraphicsItem* item = m_visuals[t];
                    t->width = item->boundingRect().width();
                    t->height = item->boundingRect().height();
                }
            }
            isResizing = true; // Start resize mode
            dragHandle = handle; // Store handle type
            resizeStartPos = pos; // Store start pos
            event->accept();
            return;
        }
    }
    if (event->button() == Qt::MiddleButton) { // Middle click
        isPanning = true;
        lastPanPos = event->pos();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    }
    if (current_tool == ToolType::SELECT) { // select mode
        QGraphicsView::mousePressEvent(event); // default handling
        updateSelectionHandles();
        initialItemPositions.clear(); // Reset move history
        QGraphicsItem* clickedItem = scene->itemAt(mapToScene(event->pos()), QTransform());
        if (clickedItem) { // If item clicked
            for (QGraphicsItem* item : scene->selectedItems()) { // Store initial pos of all selected
                QVariant data = item->data(0);
                GraphicsObject* objPtr = static_cast<GraphicsObject*>(data.value<void*>());
                std::shared_ptr<GraphicsObject> obj = nullptr;
                for(auto& s : m_shapes) { if(s.get() == objPtr) { obj = s; break; } }
                if(obj) initialItemPositions[obj] = obj->getPosition();
            }
        } else {
            emit shapeSelected(nullptr); // Deselect
        }
        updateSelectionHandles();
        return;
    }
    if (current_tool != ToolType::SELECT) scene->clearSelection(); // Deselect if drawing
    start_point = mapToScene(event->pos()); // Store draw start point
    std::unique_ptr<AddShape> addShapeCommand = nullptr; // to create command for undo purpose if a object was drawn
    if (current_tool == ToolType::RECTANGLE) { // Start Rectangle
        auto r = std::make_shared<Rectangle>();
        r->x = start_point.x();
        r->y = start_point.y();
        current_drawing_object = r;
        addShapeCommand = std::make_unique<AddShape>(this, r);
        addShape(r);
    } else if (current_tool == ToolType::CIRCLE) { // Start Circle
        auto c = std::make_shared<Circle>();
        c->cx = start_point.x();
        c->cy = start_point.y();
        current_drawing_object = c;
        addShapeCommand = std::make_unique<AddShape>(this, c);
        addShape(c);
    } else if (current_tool == ToolType::LINE) { // Start Line
        auto l = std::make_shared<Line>();
        l->x1 = start_point.x();
        l->x2 = start_point.x();
        l->y1 = start_point.y();
        l->y2 = start_point.y();
        current_drawing_object = l;
        addShapeCommand = std::make_unique<AddShape>(this, l);
        addShape(l);
    } else if (current_tool == ToolType::HEXAGON) { // Start Hexagon
        auto h = std::make_shared<Hexagon>();
        h->cx = start_point.x();
        h->cy = start_point.y();
        current_drawing_object = h;
        addShapeCommand = std::make_unique<AddShape>(this, h);
        addShape(h);
    } else if (current_tool == ToolType::FREEHAND) { // Start Freehand
        auto f = std::make_shared<FreeHand>();
        f->points.emplace_back(start_point);
        current_drawing_object = f;
        addShapeCommand = std::make_unique<AddShape>(this, f);
        addShape(f);
    } else if (current_tool == ToolType::TEXT) { // Start Text
        auto t = std::make_shared<Text>();
        t->showBorder = true;
        t->x = start_point.x();
        t->y = start_point.y();
        t->fill_color = "black";
        emptyTextItems.insert(t); // it is empty so it goes in this list
        current_drawing_object = t;
        addShapeCommand = std::make_unique<AddShape>(this, t);
        addShape(t);
    }
    if (addShapeCommand) pushCommand(std::move(addShapeCommand)); // Push command
}