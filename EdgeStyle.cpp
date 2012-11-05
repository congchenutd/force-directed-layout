#include "EdgeStyle.h"
#include "Edge.h"
#include "EdgeTopology.h"
#include "NodeTopology.h"
#include "Node.h"
#include <QRect>
#include <QPainterPath>
#include <QPainter>

namespace ForceDirectedLayout {

EdgeStyle::EdgeStyle(int width, const QColor& color)
    : _width(width),
      _color(color)
{}

void EdgeStyle::setEdge(Edge* edge) {
    _edge = edge;
}

//////////////////////////////////////////////////////////////////////////
RegularEdgeStyle::RegularEdgeStyle(int width, const QColor& color)
    : EdgeStyle(width, color) {}

QRectF RegularEdgeStyle::boundingRect() const
{
    EdgeTopology* topology = _edge->getTopology();
    if(topology == 0 || topology->isDangling())
        return QRectF();

    const qreal extra = _width / 2;   // half of the width of pen
    QPointF node1Pos = topology->getNode1()->pos();
    QPointF node2Pos = topology->getNode2()->pos();
    return QRectF(node1Pos, QSizeF(node2Pos.x() - node1Pos.x(),
                                   node2Pos.y() - node1Pos.y()))
            .normalized()
            .adjusted(-extra, -extra, extra, extra);
}

void RegularEdgeStyle::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    EdgeTopology* topology = _edge->getTopology();
    if(topology == 0 || topology->isDangling())
        return;
    painter->setPen(QPen(_color, _width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(topology->getNode1()->pos(), topology->getNode2()->pos());
}

}
