#include "Node.h"
#include "Edge.h"
#include "Engine.h"
#include "NodeTopology.h"
#include "NodeStyle.h"
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

namespace ForceDirectedLayout
{

Node::Node()
    : _pinned(false)
{
	setTopolopy(new NodeTopology);      // default topology
	setStyle   (new RoundNodeStyle);    // default style
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

void Node::setStyle(NodeStyle* style) {
	if(style != 0)
		_style = style;
}

QList<Edge*> Node::getEdges() const {
    return _topology->getEdges();
}

bool Node::isGrabbed() const {
    return scene()->mouseGrabberItem() == this;
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant& value)
{
    if(change == ItemPositionHasChanged)
    {
		foreach(Edge* edge, _topology->getEdges())   // update edges' positions
            edge->adjust();
        startEngine();
    }
    return QGraphicsItem::itemChange(change, value);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem::mouseReleaseEvent(event);
    startEngine();
}

void Node::startEngine() {
    if(Engine* engine = Engine::getCurrent())
        engine->start();
}

bool Node::advance()
{
	if(_newPos == pos())
		return false;
	setPos(_newPos);       // apply buffered position
	return true;
}

// drawing related
QRectF Node::boundingRect() const {
    return _style->boundingRect();
}
QPainterPath Node::shape() const {
    return _style->shape();
}
void Node::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    _style->paint(painter, option, widget);
}

int Node::getWidth() const {
    return _style->getWidth();
}

// topology related
bool Node::isRoot() const {
    return _topology->isRoot();
}
int Node::getSize() const {
    return _topology->getSize();
}
int Node::getLevel() const {
    return _topology->getLevel();
}
Node *Node::getParent() const {
    return _topology->getParent();
}
NodeList Node::getChildren() const {
    return _topology->getChildren();
}
NodeList Node::getAncestors() const {
	return _topology->getAncestors();
}


} // namespace
