#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsObject>

namespace ForceDirectedLayout
{

class EdgeTopology;
class EdgeStyle;
class Node;

// interface for edge
class Edge : public QGraphicsObject
{
public:
    Edge(Node* node1, Node* node2);
    void setTopology(EdgeTopology* topology);
    void setStyle   (EdgeStyle*    style);
    EdgeTopology* getTopology() const { return _topology; }
    Node* getNode1() const;
    Node* getNode2() const;
    Node* getTheOtherNode(const Node* node) const;
    void adjust();             // adjust the position according to the nodes

    virtual ~Edge() {}
    virtual QRectF boundingRect() const;
    virtual void   paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);

    virtual double getStrength() const { return 0.1; }   // base value of the force

protected:
    EdgeTopology* _topology;
    EdgeStyle*    _style;
};

}

#endif // EDGE_H
