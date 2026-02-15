#include "gui/canvas.h"

void Canvas::mouseReleaseEvent(QMouseEvent* event) {
    if (isResizing) { // finishing resize
        isResizing = false;
        event->accept();
        return;
    }
    if (event->button() == Qt::MiddleButton) { // finishing pan
        isPanning = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }
    if (current_tool == ToolType::SELECT && !isResizing) { // selection mode
        QGraphicsView::mouseReleaseEvent(event);
        if (initialItemPositions.empty()) { // No move happened
            updateSelectionHandles();
            return;
        }
        std::vector<std::pair<std::shared_ptr<GraphicsObject>, std::pair<QPointF, QPointF>>> objsData;
        for (QGraphicsItem* item : scene->selectedItems()) { // Check moved items
            QVariant data = item->data(0);
            GraphicsObject* objPtr = static_cast<GraphicsObject*>(data.value<void*>());
            std::shared_ptr<GraphicsObject> obj = nullptr;
            for(auto& s : m_shapes) { if(s.get() == objPtr) { obj = s; break; } }
            if (obj && initialItemPositions.count(obj)) { // If we tracked this object
                QPointF oldPos = initialItemPositions[obj];
                QPointF newPos;
                if (auto l = std::dynamic_pointer_cast<Line>(obj)) {
                    QGraphicsLineItem* line = dynamic_cast<QGraphicsLineItem*>(item);
                    newPos = line->mapToScene(line->line().p1());
                } else if (std::dynamic_pointer_cast<Hexagon>(obj) || std::dynamic_pointer_cast<FreeHand>(obj)) {
                    newPos = item->mapToScene(obj->getPosition());
                } else {
                    newPos = item->mapToScene(0, 0);
                }
                double dx = newPos.x() - oldPos.x();
                double dy = newPos.y() - oldPos.y();
                if (std::sqrt((dx*dx)+(dy*dy)) > 0.1) { // Only record if moved some distance
                    objsData.push_back({obj, {oldPos, newPos}}); // for undo
                    obj->move(newPos); // Update cpp model
                    updateShape(obj);
                }
                emit shapeSelected(obj);
            }
        }
        if (!objsData.empty()) { // If moves happened
            auto moveCommand = std::make_unique<Move>(this, objsData);
            pushCommand(std::move(moveCommand)); // Push undo command
        }
        return;
    }
    if (current_tool != ToolType::SELECT && current_tool != ToolType::TEXT) {
        QPointF pos = mapToScene(event->pos());
        double dx = start_point.x() - pos.x(), dy = start_point.y() - pos.y();
        // Check if movement was negligible (User just clicked)
        if (sqrt(dx*dx + dy*dy) < 0.1) {
            bool remove = false;
            // For Freehand, check if there are enough points for a valid object
            if (current_tool == ToolType::FREEHAND) {
                if (auto* f = dynamic_cast<FreeHand*>(current_drawing_object.get())) {
                    if (f->points.size() < 3) remove = true;
                }
            } else {
                remove = true;
            }
            if (remove) {
                removeShape(current_drawing_object, true);
                if (!undoStack.empty()) undoStack.pop();
            }
        }
    }
    if (current_tool == ToolType::TEXT && current_drawing_object) {
        auto t = std::dynamic_pointer_cast<Text>(current_drawing_object);
        if (t && m_visuals.count(t)) {
            QGraphicsItem* item = m_visuals[t];
            QGraphicsTextItem* textItem = dynamic_cast<QGraphicsTextItem*>(item);
            if (textItem) {
                textItem->setTextInteractionFlags(Qt::TextEditorInteraction); // Enter edit mode
                textItem->setFocus();
                textItem->setSelected(true);
                QTextCursor c = textItem->textCursor();
                c.movePosition(QTextCursor::End);
                textItem->setTextCursor(c);
            }
        }
    }
    current_drawing_object = nullptr; // clear current drawing object
    QGraphicsView::mouseReleaseEvent(event);
    updateSelectionHandles();
}