#ifndef NODE_H
#define NODE_H

#include <QGraphicsObject>

namespace ForceDirectedLayout
{

class Edge;
class NodeTopology;
class NodeStyle;

// Node in a graph
// a QGraphicsObject
// topology (connectivity) is delegated to NodeTopology,
// which associates with EdgeTopology
// drawing is delegated to NodeStyle
// itself only concerns interaction and layout
class Node : public QGraphicsObject
{
public:
    Node();
    NodeTopology* getTopology() const { return _topology; }
    void setTopolopy(NodeTopology* topology);
    QList<Edge*> getEdges() const;
    void setStyle   (NodeStyle* style);
    bool isPinned() const { return _pinned; }
    void setPinned (bool pinned) { _pinned = pinned; }
	bool isGrabbed() const;                // grabbed by mouse?

	// first call setNewPos() to buffer the new pos,
	// then advance() to apply the new pos
    // this way we can detect if it's moved
    void setNewPos(const QPointF& newPos) { _newPos = newPos; }  // buffer _newPos
    bool advance();                                              // return true if moved

    virtual QVariant     itemChange(GraphicsItemChange change, const QVariant& value);
    virtual QRectF       boundingRect() const;
    virtual QPainterPath shape()  const;
    virtual void         paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);

protected:
    NodeTopology* _topology;
    NodeStyle*    _style;
    bool    _pinned;
    QPointF _newPos;
};

}

#endif // NODE_H
