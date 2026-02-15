#include "gui/canvas.h"

Canvas::Canvas(QWidget* parent) : QGraphicsView(parent) {
    scene = std::make_unique<QGraphicsScene>(this); // Create scene
    this->setScene(scene.get()); // Set the scene for this view
    scene->setBackgroundBrush(Qt::white); // Set canvas background
    this->setFocusPolicy(Qt::StrongFocus); // Accept keyboard focus
    this->setRenderHint(QPainter::Antialiasing); // Enable smooth drawing
    current_tool = ToolType::SELECT; // Default tool
    this->setDragMode(QGraphicsView::RubberBandDrag); // Enable selection box
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse); // Zoom towards mouse
    setResizeAnchor(QGraphicsView::AnchorUnderMouse); // Resize from mouse
    setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate); // Optimize rendering
    scene->setSceneRect(0, 0, 8000, 6000); // Large canvas area
    // initialize variables
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
    clipboard.clear(); // Clear clipboard
}

void Canvas::loadShapes(const std::vector<std::shared_ptr<GraphicsObject>>& shapes) {
    reset(); // Clear existing canvas
    for (auto& obj : shapes) addShape(obj);
}

const std::vector<std::shared_ptr<GraphicsObject>>& Canvas::getShapes() const {
    return m_shapes;
}

QGraphicsScene* Canvas::getScene() const {
    return scene.get();
}