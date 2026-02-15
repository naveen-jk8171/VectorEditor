#include "model/circle.h"
#include "model/freehand.h"
#include "model/hexagon.h"
#include "model/line.h"
#include "model/rect.h"
#include "model/text.h"

QGraphicsItem* Circle::draw(QGraphicsScene* scene) {
    QGraphicsEllipseItem* item = new QGraphicsEllipseItem(-r, -r, 2*r, 2*r);
    item->setPos(cx, cy);
    QColor sColor = stroke_color.empty()? Qt::black : QColor(QString::fromStdString(stroke_color));
    item->setPen(QPen(sColor, stroke_width));
    if (!fill_color.empty()) item->setBrush(QBrush(QColor(QString::fromStdString(fill_color))));
    item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    item->setData(0, QVariant::fromValue(static_cast<void*>(this)));
    scene->addItem(item);
    return item;
}

QGraphicsItem* FreeHand::draw(QGraphicsScene* scene) {
    if (points.empty()) return nullptr;
    QPainterPath path;
    path.moveTo(points[0]);
    for (int i = 1; i < points.size(); i++) path.lineTo(points[i]);
    QGraphicsPathItem* item = new QGraphicsPathItem(path);
    QColor sColor = stroke_color.empty()? Qt::black : QColor(QString::fromStdString(stroke_color));
    item->setPen(QPen(sColor, stroke_width));
    if (!fill_color.empty()) item->setBrush(QBrush(QColor((QString::fromStdString(fill_color)))));
    item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    item->setData(0, QVariant::fromValue(static_cast<void*>(this)));
    scene->addItem(item);
    return item;
}

QGraphicsItem* Hexagon::draw(QGraphicsScene* scene) {
    QGraphicsPolygonItem* item = new QGraphicsPolygonItem(getPoints());
    QColor sColor = stroke_color.empty()? Qt::black : QColor(QString::fromStdString(stroke_color));
    item->setPen(QPen(sColor, stroke_width));
    if (!fill_color.empty()) item->setBrush(QBrush(QColor(QString::fromStdString(fill_color))));
    item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    item->setData(0, QVariant::fromValue(static_cast<void*>(this)));
    scene->addItem(item);
    return item;
}

QGraphicsItem* Line::draw(QGraphicsScene* scene) {
    QGraphicsLineItem* item = new QGraphicsLineItem(0, 0, x2-x1, y2-y1);
    item->setPos(x1, y1);
    QColor sColor = stroke_color.empty()? Qt::black : QColor(QString::fromStdString(stroke_color));
    item->setPen(QPen(sColor, stroke_width));
    item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    item->setData(0, QVariant::fromValue(static_cast<void*>(this)));
    scene->addItem(item);
    return item;
}

QGraphicsItem* Rectangle::draw(QGraphicsScene* scene) {
    QPainterPath path;
    path.addRoundedRect(0, 0, width, height, rx, ry);
    QGraphicsPathItem* item = new QGraphicsPathItem(path);
    item->setPos(x, y);
    QColor sColor = stroke_color.empty()? Qt::black : QColor(QString::fromStdString(stroke_color));
    item->setPen(QPen(sColor, stroke_width));
    if (!fill_color.empty()) item->setBrush(QBrush(QColor((QString::fromStdString(fill_color)))));
    item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    item->setData(0, QVariant::fromValue(static_cast<void*>(this)));
    scene->addItem(item);
    return item;
}

QGraphicsItem* Text::draw(QGraphicsScene* scene) {
    if (fill_color.empty()) fill_color = "black";
    QGraphicsTextItem* item = new QGraphicsTextItem(QString::fromStdString(content));
    QFont font(QString::fromStdString(fontFamily));
    font.setPixelSize(fontSize);
    item->setFont(font);
    if (!fill_color.empty() && fill_color != "none") item->setDefaultTextColor(QColor(QString::fromStdString(fill_color)));
    item->setPos(x, y);
    if (width > 0) item->setTextWidth(width);
    if (showBorder) {
        QRectF rect = item->boundingRect();
        QGraphicsRectItem* border = new QGraphicsRectItem(rect, item);
        QPen pen(Qt::DashLine);
        pen.setColor(Qt::gray);
        border->setPen(pen);
        QObject::connect(item->document(), &QTextDocument::contentsChanged, [item, border](){border->setRect(item->boundingRect());});
    }
    item->setTextInteractionFlags(Qt::NoTextInteraction);
    item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    item->setData(0, QVariant::fromValue(static_cast<void*>(this)));
    QObject::connect(item->document(), &QTextDocument::contentsChanged, [this, item]() {this->content = item->toPlainText().toStdString();});
    scene->addItem(item);
    visualItem = item;
    return item;
}