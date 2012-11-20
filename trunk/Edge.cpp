#include "Edge.h"
#include "Node.h"
#include "EdgeTopology.h"
#include "EdgeStyle.h"
#include "NodeTopology.h"
#include "Node.h"
#include <QPainter>

namespace ForceDirectedLayout
{

Edge::Edge(Node* node1, Node* node2)
{
    setTopology(new EdgeTopology(node1->getTopology(),
                                 node2->getTopology()));     // default topo
    setStyle   (new RegularEdgeStyle);                       // default style
	setZValue(qMin(_topology->getNode1()->zValue(),
				   _topology->getNode2()->zValue()) - 1);    // place behind nodes
}

void Edge::setTopology(EdgeTopology* topology)
{
    if(topology != 0)
    {
        _topology = topology;
        _topology->setEdge(this);
    }
}

void Edge::setStyle(EdgeStyle* style)
{
    if(style != 0)
    {
        _style = style;
        _style->setEdge(this);
    }
}

Node* Edge::getNode1() const {
    return getTopology()->getNode1();
}

Node* Edge::getNode2() const {
    return getTopology()->getNode2();
}

Node *Edge::getTheOtherNode(const Node* node) const {
	return getTopology()->getTheOtherNode(node);
}

bool Edge::isDangling() const {
	return _topology->isDangling();
}

QRectF Edge::boundingRect() const {
    return _style->boundingRect();
}

void Edge::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
        _style->paint(painter, option, widget);
}

void Edge::adjust() {
    if(!_topology->isDangling())
        prepareGeometryChange();    // will call update()
}


///////////////////////////////////////////////////////////////////////////////
FrameEdge::FrameEdge(int x1, int y1, int x2, int y2,
					 Direction direction, int width, const QColor &color)
    : _end1(x1, y1),
      _end2(x2, y2),
      _direction(direction),
      _width(width),
      _color(color)
{}

QPointF FrameEdge::vectorFrom(const Node* node) const
{
    return (_direction == TOP || _direction == BOTTOM) ?
                QPointF(0, _end1.y() - node->pos().y())
              : QPointF(_end1.x() - node->pos().x(), 0);
}

bool FrameEdge::isInside(const QPointF& point) const
{
    switch(_direction)
    {
    case TOP:
        return point.y() > _end1.y();
    case BOTTOM:
        return point.y() < _end1.y();
    case LEFT:
        return point.x() > _end1.x();
    default:  // RIGHT
        return point.x() < _end1.x();
    }
}

QRectF FrameEdge::boundingRect() const
{
	const qreal extra = _width / 2;   // half of the width of pen
	return QRectF(_end1, QSizeF(_end2.x() - _end1.x(),
								_end2.y() - _end1.y()))
			.normalized()
			.adjusted(-extra, -extra, extra, extra);
}

void FrameEdge::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	painter->setPen(QPen(_color, _width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter->drawLine(_end1, _end2);
}

} // namespace
