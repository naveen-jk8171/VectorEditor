#pragma once

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

enum class ToolType {
    SELECT,
    RECTANGLE,
    CIRCLE,
    LINE,
    HEXAGON,
    FREEHAND,
    TEXT,
};

class Canvas : public QGraphicsView {
    Q_OBJECT

public:
    explicit Canvas(QWidget* parent = nullptr);
    ~Canvas();
    void loadShapes(const std::vector<GraphicsObject*>& shapes);
    void addShape(GraphicsObject* shape);
    const std::vector<GraphicsObject*>& getShapes() const;
    void setTool(ToolType tool);
    QGraphicsScene* getScene() const;
    void updateShape(GraphicsObject* shape);
    void redraw();
    void refreshLayers();
    void bringToFront();
    void sendToBack();
    void copy();
    void cut();
    void paste();
    void deleteShapes();
    void removeShape(GraphicsObject* obj, bool deleteIt = true);
    int getCountItems();
    void undo();
    void redo();
    void pushCommand(Command* command);
    QGraphicsItem* getVisualItem(GraphicsObject* obj) const;
    void updateSelectionHandles();
    void selectAll();
    void reset();
    bool modified;
    std::vector<GraphicsObject*> clipboard;
signals:
    void shapeSelected(GraphicsObject* shape);

protected:
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    bool viewportEvent(QEvent* event) override;
private:
    // void onFocusChanged(QGraphicsItem* newFocus, QGraphicsItem* oldFocus, Qt::FocusReason reason);
    QGraphicsScene* scene;
    std::vector<GraphicsObject*> m_shapes;
    ToolType current_tool;
    GraphicsObject* current_drawing_object;
    QPointF start_point;
    std::unordered_map<GraphicsObject*, QGraphicsItem*> m_visuals;
    bool isPanning;
    QPoint lastPanPos;
    int pasteCount;
    SelectionHandles* selectionHandles;
    bool isResizing;
    SelectionHandles::HandlePosition dragHandle;
    QPointF resizeStartPos;
    std::stack<Command*> undoStack;
    std::stack<Command*> redoStack;
    QPointF dragStartPos;
    std::unordered_map<GraphicsObject*, QPointF> initialItemPositions;
    std::set<Text*> emptyTextItems;
};