#include "model/circle.h"
#include "model/freehand.h"
#include "model/hexagon.h"
#include "model/line.h"
#include "model/rect.h"
#include "model/text.h"

QGraphicsItem* Circle::draw(QGraphicsScene* scene) { // Implements drawing for Circle
    QGraphicsEllipseItem* item = new QGraphicsEllipseItem(-r, -r, 2*r, 2*r); // Create ellipse item with bounding box
    item->setPos(cx, cy); // Set position to center
    QColor sColor = stroke_color.empty()? Qt::black : QColor(QString::fromStdString(stroke_color)); // Set stroke color, default black
    item->setPen(QPen(sColor, stroke_width)); // Apply pen with width
    if (!fill_color.empty()) item->setBrush(QBrush(QColor(QString::fromStdString(fill_color)))); // Apply fill color if set
    item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable); // Make item movable and selectable
    item->setData(0, QVariant::fromValue(static_cast<void*>(this))); // Store pointer to model object
    scene->addItem(item); // Add to scene
    return item; // Return visual item
}

QGraphicsItem* FreeHand::draw(QGraphicsScene* scene) { // Implements drawing for Freehand sketch
    if (points.empty()) return nullptr; // Handle empty sketch
    QPainterPath path; // Create path for drawing
    path.moveTo(points[0]); // Move to start
    for (int i = 1; i < points.size(); i++) path.lineTo(points[i]); // Connect points with lines
    QGraphicsPathItem* item = new QGraphicsPathItem(path); // Create path item
    QColor sColor = stroke_color.empty()? Qt::black : QColor(QString::fromStdString(stroke_color)); // Set stroke color
    item->setPen(QPen(sColor, stroke_width)); // Set pen
    if (!fill_color.empty()) item->setBrush(QBrush(QColor((QString::fromStdString(fill_color))))); // Set brush
    item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable); // Set flags
    item->setData(0, QVariant::fromValue(static_cast<void*>(this))); // Store model pointer
    scene->addItem(item); // Add to scene
    return item; // Return item
}

QGraphicsItem* Hexagon::draw(QGraphicsScene* scene) { // Implements drawing for Hexagon
    QGraphicsPolygonItem* item = new QGraphicsPolygonItem(getPoints()); // Create polygon from points
    QColor sColor = stroke_color.empty()? Qt::black : QColor(QString::fromStdString(stroke_color)); // Set stroke color
    item->setPen(QPen(sColor, stroke_width)); // Set pen
    if (!fill_color.empty()) item->setBrush(QBrush(QColor(QString::fromStdString(fill_color)))); // Set fill
    item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable); // Set flags
    item->setData(0, QVariant::fromValue(static_cast<void*>(this))); // Store pointer
    scene->addItem(item); // Add to scene
    return item; // Return item
}

QGraphicsItem* Line::draw(QGraphicsScene* scene) { // Implements drawing for Line
    QGraphicsLineItem* item = new QGraphicsLineItem(0, 0, x2-x1, y2-y1); // Create line item
    item->setPos(x1, y1); // Set start position
    QColor sColor = stroke_color.empty()? Qt::black : QColor(QString::fromStdString(stroke_color)); // Set stroke color
    item->setPen(QPen(sColor, stroke_width)); // Set pen
    item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable); // Set flags
    item->setData(0, QVariant::fromValue(static_cast<void*>(this))); // Store pointer
    scene->addItem(item); // Add to scene
    return item; // Return item
}

QGraphicsItem* Rectangle::draw(QGraphicsScene* scene) { // Implements drawing for Rectangle
    QPainterPath path; // Create path
    path.addRoundedRect(0, 0, width, height, rx, ry); // Add rounded rect to path
    QGraphicsPathItem* item = new QGraphicsPathItem(path); // Create item
    item->setPos(x, y); // Set position
    QColor sColor = stroke_color.empty()? Qt::black : QColor(QString::fromStdString(stroke_color)); // Set stroke color
    item->setPen(QPen(sColor, stroke_width)); // Set pen
    if (!fill_color.empty()) item->setBrush(QBrush(QColor((QString::fromStdString(fill_color))))); // Set fill
    item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable); // Set flags
    item->setData(0, QVariant::fromValue(static_cast<void*>(this))); // Store pointer
    scene->addItem(item); // Add to scene
    return item; // Return item
}

QGraphicsItem* Text::draw(QGraphicsScene* scene) { // Implements drawing for Text
    if (fill_color.empty()) fill_color = "black"; // Default text color
    QGraphicsTextItem* item = new QGraphicsTextItem(QString::fromStdString(content)); // Create text item
    QFont font(QString::fromStdString(fontFamily)); // Create font
    font.setPixelSize(fontSize); // Set size
    item->setFont(font); // Apply font
    if (!fill_color.empty() && fill_color != "none") item->setDefaultTextColor(QColor(QString::fromStdString(fill_color))); // Set color
    item->setPos(x, y); // Set position
    if (width > 0) item->setTextWidth(width); // Set width limit
    if (showBorder) { // Draw edit border if active
        QRectF rect = item->boundingRect(); // Get bounds
        QGraphicsRectItem* border = new QGraphicsRectItem(rect, item); // Create border item
        QPen pen(Qt::DashLine); // Dashed line
        pen.setColor(Qt::gray); // Gray color
        border->setPen(pen); // Apply pen
        QObject::connect(item->document(), &QTextDocument::contentsChanged, [item, border](){border->setRect(item->boundingRect());}); // Update border on change
    }
    item->setTextInteractionFlags(Qt::NoTextInteraction); // Default to no interaction
    item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable); // Set flags
    item->setData(0, QVariant::fromValue(static_cast<void*>(this))); // Store pointer
    QObject::connect(item->document(), &QTextDocument::contentsChanged, [this, item]() {this->content = item->toPlainText().toStdString();}); // Sync content
    scene->addItem(item); // Add to scene
    visualItem = item; // Store visual reference
    return item; // Return item
}