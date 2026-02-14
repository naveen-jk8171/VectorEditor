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
    void loadShapes(const std::vector<std::shared_ptr<GraphicsObject>>& shapes);
    void addShape(std::shared_ptr<GraphicsObject> shape);
    const std::vector<std::shared_ptr<GraphicsObject>>& getShapes() const;
    void setTool(ToolType tool);
    QGraphicsScene* getScene() const;
    void updateShape(std::shared_ptr<GraphicsObject> shape);
    void redraw();
    void refreshLayers();
    void bringToFront();
    void sendToBack();
    void copy();
    void cut();
    void paste();
    void deleteShapes();
    void removeShape(std::shared_ptr<GraphicsObject> obj, bool deleteIt = true);
    int getCountItems();
    void undo();
    void redo();
    void pushCommand(std::unique_ptr<Command> command);
    QGraphicsItem* getVisualItem(std::shared_ptr<GraphicsObject> obj) const;
    void updateSelectionHandles();
    void selectAll();
    void reset();
    bool modified;
    std::vector<std::shared_ptr<GraphicsObject>> clipboard;
signals:
    void shapeSelected(std::shared_ptr<GraphicsObject> shape);

protected:
    void mouseDoubleClickEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    bool viewportEvent(QEvent* event) override;
private:
    std::unique_ptr<QGraphicsScene> scene;
    std::vector<std::shared_ptr<GraphicsObject>> m_shapes;
    ToolType current_tool;
    std::shared_ptr<GraphicsObject> current_drawing_object;
    QPointF start_point;
    std::unordered_map<std::shared_ptr<GraphicsObject>, QGraphicsItem*> m_visuals;
    bool isPanning;
    QPoint lastPanPos;
    int pasteCount;
    std::unique_ptr<SelectionHandles> selectionHandles;
    bool isResizing;
    SelectionHandles::HandlePosition dragHandle;
    QPointF resizeStartPos;
    std::stack<std::unique_ptr<Command>> undoStack;
    std::stack<std::unique_ptr<Command>> redoStack;
    QPointF dragStartPos;
    std::unordered_map<std::shared_ptr<GraphicsObject>, QPointF> initialItemPositions;
    std::set<Text*> emptyTextItems;
};