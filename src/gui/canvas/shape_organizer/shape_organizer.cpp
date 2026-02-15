#include "gui/canvas.h"

void Canvas::addShape(std::shared_ptr<GraphicsObject> shape) { // Adds a new shape to the model and view
    if (shape) { // Check if the shared pointer is valid
        m_shapes.push_back(shape); // Add to the main model vector
        QGraphicsItem* item = shape->draw(scene.get()); // Create the visual item on the scene
        item->setZValue(m_shapes.size()); // Set stacking order based on insertion order
        m_visuals[shape] = item; // Map the model object to its visual representation
        modified = true; // Mark canvas as modified for save prompts
        if (QGraphicsTextItem* t = dynamic_cast<QGraphicsTextItem*>(item)) { // Check if it is a text item
            QObject::connect(t->document(), &QTextDocument::contentsChanged, [this, shape]() { // Listen for text changes
                updateSelectionHandles(); // Update selection box if text size changes
            });
        }
    }
}

void Canvas::updateShape(std::shared_ptr<GraphicsObject> shape) { // Refreshes the visual representation of a shape
    if (!shape) return; // Safety check
    if (m_visuals.find(shape) != m_visuals.end()) { // Check if the shape is currently visualized
        QGraphicsItem* item = m_visuals[shape]; // Get the existing visual item
        bool wasSelected = item->isSelected(); // Remember selection state
        double zValue = item->zValue(); // Remember Z-value to preserve layer order
        scene->removeItem(item); // Remove old visual from scene
        delete item; // Free memory of the old visual item
        QGraphicsItem* newItem = shape->draw(scene.get()); // Directly draw the new item
        newItem->setZValue(zValue); // Restore original Z-value
        m_visuals[shape] = newItem; // Update the map with the new visual item
        
        if (QGraphicsTextItem* t = dynamic_cast<QGraphicsTextItem*>(newItem)) { // Re-connect text signals
            QObject::connect(t->document(), &QTextDocument::contentsChanged, [this, shape]() {
                updateSelectionHandles();
            });
        }
        newItem->setSelected(wasSelected); // Restore selection state
    }
}

void Canvas::updateSelectionHandles() { // Manages the resize/selection handles
    if (selectionHandles) { // If handles exist
        scene->removeItem(selectionHandles.get()); // Remove them from scene
        selectionHandles.reset(); // Destroy the handle object
    }
    if (scene->selectedItems().size() == 1) { // Handles only appear for single selection
        QGraphicsItem* item = scene->selectedItems().first(); // Get selected item
        QVariant data = item->data(0); // Retrieve stored model pointer
        GraphicsObject* objPtr = static_cast<GraphicsObject*>(data.value<void*>()); // Cast to raw pointer
        std::shared_ptr<GraphicsObject> obj = nullptr; // Shared pointer holder
        for(auto& s : m_shapes) { if(s.get() == objPtr) { obj = s; break; } } // Find owning shared_ptr
        if (obj) { // If found
            selectionHandles = std::make_unique<SelectionHandles>(obj, this); // Create new handles
            scene->addItem(selectionHandles.get()); // Add to scene
        }
    }
}

void Canvas::removeShape(std::shared_ptr<GraphicsObject> obj, bool deleteIt) { // Removes a shape from the canvas
    if (!obj) return; // Safety check
    if (m_visuals.count(obj)) { // If visual exists
        QGraphicsItem* item = m_visuals[obj]; // Get visual item
        scene->removeItem(item); // Remove from scene
        delete item; // Delete visual item
        m_visuals.erase(obj); // Remove from map
    }
    auto it = std::find(m_shapes.begin(), m_shapes.end(), obj); // Find in model vector
    if (it != m_shapes.end()) m_shapes.erase(it); // Remove from model
    modified = true; // Mark modified
    updateSelectionHandles(); // Refresh handles (likely clearing them)
}