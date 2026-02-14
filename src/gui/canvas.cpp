#include "gui/canvas.h"
#include "model/circle.h"
#include "model/rect.h"
#include "model/line.h"
#include "model/hexagon.h"
#include "model/freehand.h"
#include "math.h"
#include <QDebug>
#include <QList>
#include <QScrollBar>
#include <algorithm>
#include "commands/move.h"
#include "commands/paste.h"
#include "commands/delete.h"
#include "commands/cut.h"
#include <iostream>
#include <QTextCursor>
#include <QTextDocument>
#include <QCoreApplication>

Canvas::Canvas(QWidget* parent) : QGraphicsView(parent) {
    scene = new QGraphicsScene(this);

    // 2. Connect this View to that Scene
    this->setScene(scene);
    scene->setBackgroundBrush(Qt::white);
    // allow the canvas to catch key presses
    this->setFocusPolicy(Qt::StrongFocus);

    // Optional: Turn on antialiasing for smoother lines
    this->setRenderHint(QPainter::Antialiasing);

    current_tool = ToolType::SELECT;
    this->setDragMode(QGraphicsView::RubberBandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);

    // optimize scrolling performance
    setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);

    // for cordinate stability
    scene->setSceneRect(0, 0, 8000, 6000);
    modified = false;
    current_drawing_object = nullptr;
    start_point = QPointF(0, 0);
    dragStartPos = QPointF(0, 0);
    resizeStartPos = QPointF(0, 0);
    lastPanPos = QPoint(0, 0);
    isPanning = false;
    pasteCount = 0;
    selectionHandles = nullptr;
    isResizing = false;
    dragHandle = SelectionHandles::NONE;
}

Canvas::~Canvas() {
    for (auto* obj : clipboard) delete obj;
    clipboard.clear();
}

void Canvas::loadShapes(const std::vector<GraphicsObject*>& shapes) {
    reset();
    m_shapes = shapes;
    for (GraphicsObject* obj : shapes) addShape(obj);
}

void Canvas::addShape(GraphicsObject* shape) {
    if (shape) {
        m_shapes.push_back(shape);
        QGraphicsItem* item = shape->draw(scene);
        item->setZValue(m_shapes.size());
        m_visuals[shape] = item;
        modified = true;
        if (QGraphicsTextItem* t = dynamic_cast<QGraphicsTextItem*>(item)) {
            QObject::connect(t->document(), &QTextDocument::contentsChanged, [this, shape]() {
                updateSelectionHandles();
            });
        }
    }

}

const std::vector<GraphicsObject*>& Canvas::getShapes() const {
    return m_shapes;
}

void Canvas::setTool(ToolType tool) {
    current_tool = tool;
    if (tool == ToolType::SELECT) {
        this->setDragMode(QGraphicsView::RubberBandDrag);
        this->setCursor(Qt::ArrowCursor);
    } else {
        this->setDragMode(QGraphicsView::NoDrag);
        this->setCursor(Qt::CrossCursor);
    }
}

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
            GraphicsObject* obj = static_cast<GraphicsObject*>(data.value<void*>());
            if (Text* t = dynamic_cast<Text*>(obj)) {
                t->showBorder = false;
                updateShape(t);
                if (t->content.empty()) emptyTextItems.insert(t);
            }
        }
    }
    for (Text* t : emptyTextItems) {
        if (t->content.empty()) {
            removeShape(t, true);
        }
    }
    emptyTextItems.clear();
    if (selectionHandles && current_tool == ToolType::SELECT) {
        QPointF pos = mapToScene(event->pos());
        SelectionHandles::HandlePosition handle = selectionHandles->getHandleAt(pos);
        if (handle != SelectionHandles::NONE) {
            GraphicsObject* obj = selectionHandles->targetShape;
            if (auto* t = dynamic_cast<Text*>(obj)) {
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
                GraphicsObject* obj = static_cast<GraphicsObject*>(data.value<void*>());
                initialItemPositions[obj] = obj->getPosition();
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
        Rectangle* r = new Rectangle();
        r->x = start_point.x();
        r->y = start_point.y();
        r->width = 0;
        r->height = 0;
        r->stroke_color = "black";
        r->fill_color = "transparent";
        r->stroke_width = 2;
        current_drawing_object = r;
        addShape(r);
    } else if (current_tool == ToolType::CIRCLE) {
        Circle* c = new Circle();
        c->cx = start_point.x();
        c->cy = start_point.y();
        c->r = 0;
        c->stroke_color = "black";
        c->stroke_width = 2;
        c->width = 0;
        c->height = 0;
        c->fill_color = "transparent";
        current_drawing_object = c;
        addShape(c);
    } else if (current_tool == ToolType::LINE) {
        Line* l = new Line();
        l->x1 = start_point.x();
        l->x2 = start_point.x();
        l->y1 = start_point.y();
        l->y2 = start_point.y();
        l->stroke_color = "black";
        l->stroke_width = 2;
        current_drawing_object = l;
        addShape(l);
    } else if (current_tool == ToolType::HEXAGON) {
        Hexagon* h = new Hexagon();
        h->cx = start_point.x();
        h->cy = start_point.y();
        h->r = 0;
        h->stroke_color = "black";
        h->stroke_width = 2;
        h->width = 0;
        h->height = 0;
        h->fill_color = "transparent";
        current_drawing_object = h;
        addShape(h);
    } else if (current_tool == ToolType::FREEHAND) {
        FreeHand* f = new FreeHand();
        f->points.emplace_back(start_point);
        f->stroke_color = "black";
        f->stroke_width = 2;
        current_drawing_object = f;
        addShape(f);
    } else if (current_tool == ToolType::TEXT) {
        Text* t = new Text();
        t->showBorder = true;
        t->x = start_point.x();
        t->y = start_point.y();
        t->fontSize = 10;
        t->fill_color = "black";
        t->content = "";
        emptyTextItems.insert(t);
        current_drawing_object = t;
        addShape(t);
    }
}

void Canvas::mouseMoveEvent(QMouseEvent* event) {
    if (isResizing && selectionHandles) {
        QPointF currPos = mapToScene(event->pos());
        GraphicsObject* obj = selectionHandles->targetShape;
        if (auto* r = dynamic_cast<Rectangle*>(obj)) {
            r->resizeShape(currPos, resizeStartPos, dragHandle);
        } else if (auto* c = dynamic_cast<Circle*>(obj)) {
            c->resizeShape(currPos, resizeStartPos, dragHandle);
        } else if (auto* l = dynamic_cast<Line*>(obj)) {
            l->resizeShape(currPos, resizeStartPos, dragHandle);
        } else if (auto* h = dynamic_cast<Hexagon*>(obj)) {
            h->resizeShape(currPos, resizeStartPos, dragHandle);
        } else if (auto* t = dynamic_cast<Text*>(obj)) {
            t->resizeShape(currPos, resizeStartPos, dragHandle);
        }
        resizeStartPos = currPos;
        updateShape(obj);
        selectionHandles->updateHandles();
        emit shapeSelected(obj);
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
        Rectangle* r = dynamic_cast<Rectangle*>(current_drawing_object);
        if (r) {
            double minX = std::min(start_point.x(), current_point.x());
            double minY = std::min(start_point.y(), current_point.y());
            r->x = minX;
            r->y = minY;
            r->width = std::abs(start_point.x()-current_point.x());
            r->height = std::abs(start_point.y()-current_point.y());
        }
    } else if (current_tool == ToolType::CIRCLE) {
        Circle* c = dynamic_cast<Circle*>(current_drawing_object);
        if (c) {
            double dx = current_point.x()-start_point.x();
            double dy = current_point.y()-start_point.y();
            c->r = std::sqrt(dx*dx + dy*dy);
            c->width = 2*c->r;
            c->height = c->width;
        }
    } else if (current_tool == ToolType::LINE) {
        Line* l = dynamic_cast<Line*>(current_drawing_object);
        if (l) {
            l->x2 = current_point.x();
            l->y2 = current_point.y();
        }
    } else if (current_tool == ToolType::HEXAGON) {
        Hexagon* h = dynamic_cast<Hexagon*>(current_drawing_object);
        if (h) {
            double dx = current_point.x()-start_point.x();
            double dy = current_point.y()-start_point.y();
            h->r = std::sqrt(dx*dx + dy*dy);
            h->width = h->r;
            h->height = h->r;
        }
    } else if (current_tool == ToolType::FREEHAND) {
        FreeHand* f = dynamic_cast<FreeHand*>(current_drawing_object);
        if (f) {
            f->points.push_back(current_point);
        }
    } else if (current_tool == ToolType::TEXT) {
        Text* t = dynamic_cast<Text*>(current_drawing_object);
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
        std::vector<std::pair<GraphicsObject*, std::pair<QPointF, QPointF>>> objsData;
        for (QGraphicsItem* item : scene->selectedItems()) {
            QVariant data = item->data(0);
            GraphicsObject* obj = static_cast<GraphicsObject*>(data.value<void*>());
            if (obj && initialItemPositions.count(obj)) {
                QPointF oldPos = initialItemPositions[obj];
                QPointF newPos;
                if (Line* l = dynamic_cast<Line*>(obj)) {
                    QGraphicsLineItem* line = dynamic_cast<QGraphicsLineItem*>(item);
                    newPos = line->mapToScene(line->line().p1());
                } else if (dynamic_cast<Hexagon*>(obj) || dynamic_cast<FreeHand*>(obj)) {
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
            Move* moveCommand = new Move(this, objsData);
            pushCommand(moveCommand);
        }
        return;
    }
    if (current_tool == ToolType::TEXT && current_drawing_object) {
        Text* t = dynamic_cast<Text*>(current_drawing_object);
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

void Canvas::mouseDoubleClickEvent(QMouseEvent* event) {
    QGraphicsItem* item = itemAt(event->pos());
    if (item) {
        QGraphicsTextItem* ti = dynamic_cast<QGraphicsTextItem*>(item);
        if (ti) {
            QVariant data = ti->data(0);
            GraphicsObject* obj = static_cast<GraphicsObject*>(data.value<void*>());
            Text* t = dynamic_cast<Text*>(obj);
            if (t) {
                t->showBorder = true;
                updateShape(t);
                item = m_visuals[t];
                ti = dynamic_cast<QGraphicsTextItem*>(item);
            }
            ti->setTextInteractionFlags(Qt::TextEditorInteraction);
            ti->setFocus();
            QGraphicsView::mouseDoubleClickEvent(event);
            return;
        }
    }
}

QGraphicsScene* Canvas::getScene() const {
    return scene;
}

void Canvas::updateShape(GraphicsObject* shape) {
    if (!shape) return;
    if (m_visuals.find(shape) != m_visuals.end()) {
        QGraphicsItem* item = m_visuals[shape];
        bool wasSelected = item->isSelected();
        double zValue = item->zValue();
        scene->removeItem(item);
        delete item;
        addShape(shape);
        QGraphicsItem* newItem = m_visuals[shape];
        newItem->setSelected(wasSelected);
    }
}

void Canvas::keyPressEvent(QKeyEvent* event) {
    QGraphicsItem* focusItem = scene->focusItem();
    if (focusItem) {
        QGraphicsTextItem* ti = dynamic_cast<QGraphicsTextItem*>(focusItem);
        if (ti && (ti->textInteractionFlags() & Qt::TextEditorInteraction)) {
            QGraphicsView::keyPressEvent(event);
            return;
        }
    }
    if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace) {
        deleteShapes();
    } else {
        QGraphicsView::keyPressEvent(event);
    }
}

void Canvas::redraw() {
    scene->clear();
    m_visuals.clear();
    for (GraphicsObject* obj : m_shapes) {
        QGraphicsItem* item = obj->draw(scene);
        m_visuals[obj] = item;
    }
}

void Canvas::refreshLayers() {
    for (int i = 0; i < m_shapes.size(); i++) {
        m_visuals[m_shapes[i]]->setZValue(i);
    }
}

void Canvas::bringToFront() {
    if (scene->selectedItems().isEmpty()) return;
    QGraphicsItem* item = scene->selectedItems().first();
    QVariant data = item->data(0);
    GraphicsObject* obj = static_cast<GraphicsObject*>(data.value<void*>());
    if (obj) {
        auto it = std::find(m_shapes.begin(), m_shapes.end(), obj);
        if (it != m_shapes.end() && it != m_shapes.end()-1) {
            m_shapes.erase(it);
            m_shapes.push_back(obj);
            refreshLayers();
            if (m_visuals.find(obj) != m_visuals.end()) {
                m_visuals[obj]->setSelected(true);
            }
        }
    }
}

void Canvas::sendToBack() {
    if (scene->selectedItems().isEmpty()) return;
    QGraphicsItem* item = scene->selectedItems().first();
    QVariant data = item->data(0);
    GraphicsObject* obj = static_cast<GraphicsObject*>(data.value<void*>());
    if (obj) {
        auto it = std::find(m_shapes.begin(), m_shapes.end(), obj);
        if (it != m_shapes.end() && it != m_shapes.begin()) {
            m_shapes.erase(it);
            m_shapes.insert(m_shapes.begin(), obj);
            refreshLayers();
            if (m_visuals.find(obj) != m_visuals.end()) {
                m_visuals[obj]->setSelected(true);
            }
        }
    }
}

void Canvas::wheelEvent(QWheelEvent* event) {
    if (event->modifiers() & Qt::ControlModifier) {
        const double sFactor = 1.09;
        if (event->angleDelta().y() > 0) scale(sFactor, sFactor); else scale(1.0/sFactor, 1.0/sFactor);
        event->accept();
    } 
    else {
        QGraphicsView::wheelEvent(event);
    }
}

void Canvas::copy() {
    for (auto& obj : clipboard) delete obj;
    clipboard.clear();
    pasteCount = 1;
    QList<QGraphicsItem*> items = scene->selectedItems();
    for (auto& item : items) {
        QVariant data = item->data(0);
        GraphicsObject* shape = static_cast<GraphicsObject*>(data.value<void*>());
        if (shape) {
            clipboard.push_back(shape->clone());
        }
    }
}

void Canvas::cut() {
    for (auto& obj : clipboard) delete obj;
    clipboard.clear();
    pasteCount = 1;
    QList<QGraphicsItem*> items = scene->selectedItems();
    std::vector<GraphicsObject*> objs;
    for (auto& item : items) {
        QVariant data = item->data(0);
        GraphicsObject* shape = static_cast<GraphicsObject*>(data.value<void*>());
        if (shape) {
            clipboard.push_back(shape->clone());
            objs.push_back(shape);
            removeShape(shape, false);
        }
    }
    if (!objs.empty()) {
        Cut* cutCommand = new Cut(this, objs);
        pushCommand(cutCommand);
    }
}

void Canvas::paste() {
    if (clipboard.empty()) return;
    // clear the selectrion so to select the newly pasted items
    scene->clearSelection();
    std::vector<GraphicsObject*> objs;
    for (GraphicsObject* shape : clipboard) {
        GraphicsObject* newShape = shape->clone();
        objs.emplace_back(newShape);
        // slight offset to user know it got pasted
        int offset = 20*pasteCount;
        if (auto* r = dynamic_cast<Rectangle*>(newShape)) {
            r->x += offset;
            r->y += offset;
        } else if (auto* c = dynamic_cast<Circle*>(newShape)) {
            c->cx += offset;
            c->cy += offset;
        } else if (auto* l = dynamic_cast<Line*>(newShape)) {
            l->x1 += offset;
            l->x2 += offset;
            l->y1 += offset;
            l->y2 += offset;
        } else if (auto* h = dynamic_cast<Hexagon*>(newShape)) {
            h->cx += offset;
            h->cy += offset;
        }
        addShape(newShape);
        // make the newly drawn shape selected
        m_visuals[newShape]->setSelected(true);
    }
    if (!objs.empty()) {
        Paste* pasteCommand = new Paste(this, objs);
        pushCommand(pasteCommand);
    }
    updateSelectionHandles();
    pasteCount++;
}

void Canvas::deleteShapes() {
    std::vector<GraphicsObject*> objs;
    for (QGraphicsItem* item : scene->selectedItems()) {
        if (!item || !item->scene()) continue;
        QVariant data = item->data(0);
        GraphicsObject* obj = static_cast<GraphicsObject*>(data.value<void*>());
        if (obj) {
            removeShape(obj, false);
            objs.emplace_back(obj);
        }
        delete item;
    }
    Delete* deleteCommand = new Delete(this, objs);
    pushCommand(deleteCommand);
    updateSelectionHandles();
    emit shapeSelected(nullptr);
}


int Canvas::getCountItems() {
    return m_shapes.size();
}

void Canvas::updateSelectionHandles() {
    if (selectionHandles) {
        scene->removeItem(selectionHandles);
        delete selectionHandles;
        selectionHandles = nullptr;
    }
    if (scene->selectedItems().size() == 1) {
        QGraphicsItem* item = scene->selectedItems().first();
        QVariant data = item->data(0);
        GraphicsObject* obj = static_cast<GraphicsObject*>(data.value<void*>());
        if (obj) {
            selectionHandles = new SelectionHandles(obj, this);
            scene->addItem(selectionHandles);
        }
    }
}

QGraphicsItem* Canvas::getVisualItem(GraphicsObject* obj) const {
    if (m_visuals.count(obj)) {
        return m_visuals.at(obj);
    }
    return nullptr;
}

void Canvas::removeShape(GraphicsObject* obj, bool deleteIt) {
    if (!obj) return;
    if (m_visuals.count(obj)) {
        QGraphicsItem* item = m_visuals[obj];
        scene->removeItem(item);
        if (deleteIt) delete item;
        m_visuals.erase(obj);
    }
    auto it = std::find(m_shapes.begin(), m_shapes.end(), obj);
    if (it != m_shapes.end()) m_shapes.erase(it);
    if (deleteIt) delete obj;
    modified = true;
    updateSelectionHandles();
}

void Canvas::undo() {
    if (undoStack.empty()) return;
    Command* command = undoStack.top();
    if (Paste* p = dynamic_cast<Paste*>(command)) pasteCount = std::max(0, pasteCount-1);
    undoStack.pop();
    command->undo();
    redoStack.push(command);
}

void Canvas::redo() {
    if (redoStack.empty()) return;
    Command* command = redoStack.top();
    if (Paste* p = dynamic_cast<Paste*>(command)) pasteCount++;
    redoStack.pop();
    command->redo();
    undoStack.push(command);
}

void Canvas::pushCommand(Command* command) {
    undoStack.push(command);
    while (!redoStack.empty()) {
        Command* command = redoStack.top();
        redoStack.pop();
        delete command;
    }
}

void Canvas::selectAll() {
    for (GraphicsObject* obj : m_shapes) m_visuals[obj]->setSelected(true);
}

void Canvas::reset() {
    current_drawing_object = nullptr;
    if (selectionHandles) {
        delete selectionHandles;
        selectionHandles = nullptr;
    }
    for (GraphicsObject* obj : m_shapes) delete obj;
    m_shapes.clear();
    m_visuals.clear();
    scene->clear();
    modified = false;
}

bool Canvas::viewportEvent(QEvent* event) {
    if (event->type() == QEvent::NativeGesture) {
        QNativeGestureEvent* gesture = static_cast<QNativeGestureEvent*>(event);
        if (gesture->gestureType() == Qt::ZoomNativeGesture) {
            qreal scaleFactor = 1.0 + gesture->value();
            scale(scaleFactor, scaleFactor);
            return true;
        }
    }
    return QGraphicsView::viewportEvent(event);
}