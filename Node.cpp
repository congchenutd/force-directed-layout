#include "Node.h"
#include "Edge.h"
#include "Engine.h"
#include "NodeTopology.h"
#include "NodeStyle.h"
#include "EdgeTopology.h"
#include <QPen>
#include <QPainter>
#include <QRadialGradient>
#include <QGraphicsScene>

namespace ForceDirectedLayout
{

//////////////////////////////////////////////////////////////////////////////////////
Node::Node()
    : _pinned(false)
{
    setTopolopy(new NodeTopology);    // default topology
    setStyle   (new RoundNodeStyle);
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
}

void Node::setTopolopy(NodeTopology* topology)
{
    if(topology != 0)
    {
        _topology = topology;
        _topology->setNode(this);
    }
}

QList<Edge*> Node::getEdges() const {
    return _topology->getEdges();
}

void Node::setStyle(NodeStyle* style) {
    if(style != 0)
        _style = style;
}

bool Node::isGrabbed() const {
    return scene()->mouseGrabberItem() == this;
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant& value)
{
    if(change == ItemPositionHasChanged)
    {
        foreach(Edge* edge, _topology->getEdges())   // update edges' position
            edge->adjust();
        if(Engine* engine = Engine::getCurrent())
            engine->start();
    }
    return QGraphicsItem::itemChange(change, value);
}

QRectF Node::boundingRect() const {
    return _style->boundingRect();
}

QPainterPath Node::shape() const {
    return _style->shape();
}

void Node::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    _style->paint(painter, option, widget);
}

bool Node::advance()
{
    if(_newPos == pos())
        return false;
    setPos(_newPos);       // apply buffered position
    return true;
}


}
