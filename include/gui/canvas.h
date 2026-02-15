#pragma once
#include <memory>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QKeyEvent>
#include <stack>
#include <QWheelEvent>
#include <vector>
#include <unordered_map>
#include "commands/command.h"
#include "model/graphics_object.h"
#include "gui/selection_handle.h"
#include "model/text.h"
#include <set>
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
#include "commands/add_shape.h"

enum class ToolType { // the tools available in the editor
    SELECT, RECTANGLE, CIRCLE, LINE, HEXAGON, FREEHAND, TEXT,
};

class Canvas : public QGraphicsView {
    Q_OBJECT

public:
    explicit Canvas(QWidget* parent = nullptr);
    ~Canvas();
    void loadShapes(const std::vector<std::shared_ptr<GraphicsObject>>& shapes); // Loads a list of shapes onto the canvas
    void addShape(std::shared_ptr<GraphicsObject> shape); // Adds a single shape to the canvas
    const std::vector<std::shared_ptr<GraphicsObject>>& getShapes() const; // Returns the list of all shapes
    void setTool(ToolType tool); // Sets the current active tool
    QGraphicsScene* getScene() const; // Returns the graphics scene
    void updateShape(std::shared_ptr<GraphicsObject> shape); // Updates the visual representation of a shape
    void redraw(); // Redraws all shapes on the canvas
    void refreshLayers(); // Updates the z-order of shapes
    void bringToFront(); // Brings selected shape to the front
    void sendToBack(); // Sends selected shape to the back
    void copy(); // Copies selected shapes to clipboard
    void cut(); // Cuts selected shapes to clipboard
    void paste(); // Pastes shapes from clipboard
    void deleteShapes(); // Deletes selected shapes
    void removeShape(std::shared_ptr<GraphicsObject> obj, bool deleteIt = true); // Removes a shape from the canvas
    int getCountItems(); // Returns the count of items on canvas
    void undo(); // Undoes the last command
    void redo(); // Redoes the last undone command
    void pushCommand(std::unique_ptr<Command> command); // Pushes a command to the undo stack
    QGraphicsItem* getVisualItem(std::shared_ptr<GraphicsObject> obj) const; // Gets the QGraphicsItem for a model object
    void updateSelectionHandles(); // Updates handles for selected object
    void selectAll(); // Selects all objects on canvas
    void reset(); // Resets the canvas state
    bool modified; // Flag to track if canvas has unsaved changes
signals:
    void shapeSelected(std::shared_ptr<GraphicsObject> shape); // Signal emitted when a shape is selected
protected:
    void mouseDoubleClickEvent(QMouseEvent* event) override; // Handles mouse double clicks
    void mousePressEvent(QMouseEvent* event) override; // Handles mouse press events
    void mouseMoveEvent(QMouseEvent* event) override; // Handles mouse move events
    void mouseReleaseEvent(QMouseEvent* event) override; // Handles mouse release events
    void keyPressEvent(QKeyEvent* event) override; // Handles key press events
    void wheelEvent(QWheelEvent* event) override; // Handles mouse wheel events (zooming)
    bool viewportEvent(QEvent* event) override; // Handles viewport events (gestures)
private:
    std::vector<std::shared_ptr<GraphicsObject>> clipboard; // Clipboard for copy/paste operations
    std::unique_ptr<QGraphicsScene> scene; // Unique pointer to the graphics scene
    std::vector<std::shared_ptr<GraphicsObject>> m_shapes; // List of all graphics objects
    ToolType current_tool; // Currently selected tool
    std::shared_ptr<GraphicsObject> current_drawing_object; // Object currently being drawn
    QPointF start_point; // Start point of the current drawing action
    std::unordered_map<std::shared_ptr<GraphicsObject>, QGraphicsItem*> m_visuals; // Map from cpp objects to visual items
    bool isPanning; // Flag for panning state
    QPoint lastPanPos; // Last position during panning
    int pasteCount; // Counter for offsetting pasted items
    std::unique_ptr<SelectionHandles> selectionHandles; // Handles for resizing
    bool isResizing; // Flag for resizing state
    SelectionHandles::HandlePosition dragHandle; // Currently active resize handle
    QPointF resizeStartPos; // Start position of resize action
    std::stack<std::unique_ptr<Command>> undoStack; // undo stack
    std::stack<std::unique_ptr<Command>> redoStack; // redo stack
    QPointF dragStartPos; // Start position of drag action
    std::unordered_map<std::shared_ptr<GraphicsObject>, QPointF> initialItemPositions; // Initial positions for move command
    std::set<std::shared_ptr<GraphicsObject>> emptyTextItems; // Track empty text items to clean up
};