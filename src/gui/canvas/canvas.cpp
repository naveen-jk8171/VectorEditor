#include "gui/canvas.h"

Canvas::Canvas(QWidget* parent) : QGraphicsView(parent) {
    scene = std::make_unique<QGraphicsScene>(this);
    this->setScene(scene.get());
    scene->setBackgroundBrush(Qt::white);
    this->setFocusPolicy(Qt::StrongFocus);
    this->setRenderHint(QPainter::Antialiasing);
    current_tool = ToolType::SELECT;
    this->setDragMode(QGraphicsView::RubberBandDrag);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);
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
    clipboard.clear();
}

void Canvas::loadShapes(const std::vector<std::shared_ptr<GraphicsObject>>& shapes) {
    reset();
    m_shapes = shapes;
    for (auto& obj : shapes) addShape(obj);
}

const std::vector<std::shared_ptr<GraphicsObject>>& Canvas::getShapes() const {
    return m_shapes;
}

QGraphicsScene* Canvas::getScene() const {
    return scene.get();
}