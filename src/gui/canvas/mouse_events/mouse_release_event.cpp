#include "gui/canvas.h"

void Canvas::mouseReleaseEvent(QMouseEvent* event) {
    if (isResizing) {
        isResizing = false;
        event->accept();
        return;
    }
    if (event->button() == Qt::MiddleButton) {
        isPanning = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }
    if (current_tool == ToolType::SELECT && !isResizing) {
        QGraphicsView::mouseReleaseEvent(event);
        if (initialItemPositions.empty()) {
            updateSelectionHandles();
            return;
        }
        std::vector<std::pair<std::shared_ptr<GraphicsObject>, std::pair<QPointF, QPointF>>> objsData;
        for (QGraphicsItem* item : scene->selectedItems()) {
            QVariant data = item->data(0);
            GraphicsObject* objPtr = static_cast<GraphicsObject*>(data.value<void*>());
            std::shared_ptr<GraphicsObject> obj = nullptr;
            for(auto& s : m_shapes) { if(s.get() == objPtr) { obj = s; break; } }
            if (obj && initialItemPositions.count(obj)) {
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
                if (std::sqrt((dx*dx)+(dy*dy)) > 0.1) {
                    objsData.push_back({obj, {oldPos, newPos}});
                    obj->move(newPos);
                    updateShape(obj);
                }
                emit shapeSelected(obj);
            }
        }
        if (!objsData.empty()) {
            auto moveCommand = std::make_unique<Move>(this, objsData);
            pushCommand(std::move(moveCommand));
        }
        return;
    }
    if (current_tool == ToolType::TEXT && current_drawing_object) {
        auto t = std::dynamic_pointer_cast<Text>(current_drawing_object);
        if (t && m_visuals.count(t)) {
            QGraphicsItem* item = m_visuals[t];
            QGraphicsTextItem* textItem = dynamic_cast<QGraphicsTextItem*>(item);
            if (textItem) {
                textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
                textItem->setFocus();
                textItem->setSelected(true);
                QTextCursor c = textItem->textCursor();
                c.movePosition(QTextCursor::End);
                textItem->setTextCursor(c);
            }
        }
    }
    current_drawing_object = nullptr;
    QGraphicsView::mouseReleaseEvent(event);
    updateSelectionHandles();
}