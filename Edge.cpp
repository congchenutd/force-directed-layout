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
    setTopology(new EdgeTopology(node1->getTopology(), node2->getTopology()));
    setStyle   (new RegularEdgeStyle);
    setZValue(qMin(_topology->getNode1()->zValue(), _topology->getNode2()->zValue()) - 1);
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


}