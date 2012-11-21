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

protected:
    EdgeTopology* _topology;
    EdgeStyle*    _style;
};


// edge of a rectangular frame, which defines the boundary of a graph
// NOTE: I assume the frame is rectangular and the edges are orthogonal to the coordinates
class FrameEdge : public QGraphicsObject
{
public:
	typedef enum {TOP, BOTTOM, LEFT, RIGHT} Direction;

public:
	FrameEdge(int x1, int y1, int x2, int y2, Direction direction,
			  int width = 1, const QColor& color = QColor(Qt::black));
    QPointF vectorFrom(const Node* node) const;    // a vector from the node to the edge
    bool    isInside(const QPointF& point) const;  // is the point on the inner side of the frame?

	virtual QRectF boundingRect() const;
	virtual void   paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);

protected:
	int       _width;
	QColor    _color;
	QPointF   _end1;
	QPointF   _end2;
	Direction _direction;
};

} // namespace

#endif // EDGE_H
