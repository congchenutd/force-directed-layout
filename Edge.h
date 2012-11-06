#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsObject>

namespace ForceDirectedLayout
{

class EdgeTopology;
class EdgeStyle;
class Node;

// edge of a graph
// a QGraphicsObject
// topology (connectivity) is delegated to EdgeTopology,
// which associates with NodeTopology
// drawing is delegated to EdgeStyle
// itself only concerns interaction and layout
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
	bool  isDangling() const;
    void  adjust();             // adjust the position according to the nodes

    virtual ~Edge() {}
    virtual QRectF boundingRect() const;
    virtual void   paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);
    virtual qreal  getStrength() const { return 0.2; }   // base value of the force

protected:
    EdgeTopology* _topology;
    EdgeStyle*    _style;
};

}

#endif // EDGE_H
