#include "gui/canvas.h"

void Canvas::addShape(std::shared_ptr<GraphicsObject> shape) {
    if (shape) {
        m_shapes.push_back(shape);
        QGraphicsItem* item = shape->draw(scene.get());
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

void Canvas::updateShape(std::shared_ptr<GraphicsObject> shape) {
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

void Canvas::updateSelectionHandles() {
    if (selectionHandles) {
        scene->removeItem(selectionHandles.get());
        selectionHandles.reset();
    }
    if (scene->selectedItems().size() == 1) {
        QGraphicsItem* item = scene->selectedItems().first();
        QVariant data = item->data(0);
        GraphicsObject* objPtr = static_cast<GraphicsObject*>(data.value<void*>());
        std::shared_ptr<GraphicsObject> obj = nullptr;
        for(auto& s : m_shapes) { if(s.get() == objPtr) { obj = s; break; } }
        if (obj) {
            selectionHandles = std::make_unique<SelectionHandles>(obj, this);
            scene->addItem(selectionHandles.get());
        }
    }
}

void Canvas::removeShape(std::shared_ptr<GraphicsObject> obj, bool deleteIt) {
    if (!obj) return;
    if (m_visuals.count(obj)) {
        QGraphicsItem* item = m_visuals[obj];
        scene->removeItem(item);
        delete item;
        m_visuals.erase(obj);
    }
    auto it = std::find(m_shapes.begin(), m_shapes.end(), obj);
    if (it != m_shapes.end()) m_shapes.erase(it);
    modified = true;
    updateSelectionHandles();
}