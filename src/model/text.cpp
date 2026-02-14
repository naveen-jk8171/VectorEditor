#include "model/text.h"
#include <QGraphicsTextItem>
#include <QFont>
#include <QTextDocument>
#include <QVariant>
#include <QFontMetricsF>

void Text::setAttribute(const std::string& key, const std::string& value) {
    if (key == "x") {
        x = std::stod(value);
    } else if (key == "y") {
        y = std::stod(value);
    } else if (key == "content") {
        content = value;
    } else if (key == "font-family") {
        fontFamily = value;
    } else if (key == "font-size") {
        fontSize = std::stoi(value);
    } else if (key == "width") {
        width = std::stod(value);
    } else {
        GraphicsObject::setAttribute(key, value);
    }
}

QGraphicsItem* Text::draw(QGraphicsScene* scene) {
    if (fill_color.empty()) fill_color = "black";
    QGraphicsTextItem* item = new QGraphicsTextItem(QString::fromStdString(content));
    QFont font(QString::fromStdString(fontFamily));
    font.setPixelSize(fontSize);
    item->setFont(font);
    if (!fill_color.empty() && fill_color != "none") {
        item->setDefaultTextColor(QColor(QString::fromStdString(fill_color)));
    }
    item->setPos(x, y);
    if (width > 0) {
        item->setTextWidth(width);
    }
    if (showBorder) {
        QRectF rect = item->boundingRect();
        QGraphicsRectItem* border = new QGraphicsRectItem(rect, item);
        QPen pen(Qt::DashLine);
        pen.setColor(Qt::gray);
        border->setPen(pen);
        QObject::connect(item->document(), &QTextDocument::contentsChanged, [item, border](){
            border->setRect(item->boundingRect());
        });
    }
    item->setTextInteractionFlags(Qt::NoTextInteraction);
    item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    item->setData(0, QVariant::fromValue(static_cast<void*>(this)));
    QObject::connect(item->document(), &QTextDocument::contentsChanged, [this, item]() {
        this->content = item->toPlainText().toStdString();
    });
    scene->addItem(item);
    visualItem = item;
    return item;
}

GraphicsObject* Text::clone() const {
    return new Text(*this);
}

std::string Text::toSVG() const {
    return "<text x=\"" + std::to_string(x) + "\" y=\"" + std::to_string(y) + "\" width=\"" + std::to_string(width) + "\" font-family=\"" + fontFamily + "\"" + " font-size=\"" + std::to_string(fontSize) + "\" stroke=\"" + stroke_color + "\" stroke-width=\"" + std::to_string(stroke_width) + "\"" + " fill=\"" + (fill_color.empty() ? "black" : fill_color) + "\">"  + content  + "</text>";
}

void Text::move(const QPointF& pos) {
    x = pos.x();
    y = pos.y();
}

QPointF Text::getPosition() const {
    return QPointF(x, y);
}

void Text::resizeShape(const QPointF& currPos, const QPointF& lastPos, SelectionHandles::HandlePosition handle) {
    double dx = currPos.x()-lastPos.x();
    double dy = currPos.y()-lastPos.y();
    double px = x + width;
    double py = y + height;
    int lineCount = visualItem->document()->lineCount();
    double h1 = height/(double)lineCount;
    if (handle == SelectionHandles::TOPLEFT) {
        width -= dx;
        height -= dy;
    } else if (handle == SelectionHandles::TOP) {
        height -= dy;
    } else if (handle == SelectionHandles::TOPRIGHT) {
        width += dx;
        height -= dy;
    } else if (handle == SelectionHandles::LEFT) {
        width -= dx;
    } else if (handle == SelectionHandles::RIGHT) {
        width += dx;
    } else if (handle == SelectionHandles::BOTTOMLEFT) {
        width -= dx;
        height += dy;
    } else if (handle == SelectionHandles::BOTTOM) {
        height += dy;
    } else if (handle == SelectionHandles::BOTTOMRIGHT) {
        width += dx;
        height += dy;
    }
    if (width < 10) width = 10;
    if (height < 10) height = 10;
    if (handle == SelectionHandles::TOPLEFT || handle == SelectionHandles::LEFT || handle == SelectionHandles::BOTTOMLEFT) x = px-width;
    if (handle == SelectionHandles::TOPLEFT || handle == SelectionHandles::TOP || handle == SelectionHandles::TOPRIGHT) y = py-height;
    double h2 = height/(double)lineCount;
    double newSize = h2/1.18;
    fontSize = std::max(10, (int)newSize);
}