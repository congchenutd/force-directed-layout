#ifndef EDGESTYLE_H
#define EDGESTYLE_H

#include <QColor>

class QRectF;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;

namespace ForceDirectedLayout {

class Edge;
class EdgeStyle
{
public:
    EdgeStyle(int width, const QColor& color);
    void setEdge(Edge* edge);

    virtual QRectF boundingRect() const = 0;
    virtual void   paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) = 0;

protected:
    Edge*  _edge;
    int    _width;
    QColor _color;
};

class RegularEdgeStyle : public EdgeStyle
{
public:
    RegularEdgeStyle(int width = 2, const QColor& color = QColor(Qt::black));

    virtual QRectF boundingRect() const;
    virtual void   paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);
};

}

#endif // EDGESTYLE_H
