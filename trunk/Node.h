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
    typedef QList<Node*> NodeList;

public:
    Node();
    NodeTopology* getTopology() const { return _topology; }
    void setTopolopy(NodeTopology* topology);
    QList<Edge*> getEdges() const;
    void setStyle   (NodeStyle* style);

	// first call setNewPos() to buffer the new pos,
	// then advance() to apply the new pos
    // this way we can detect if it's moved
    void setNewPos(const QPointF& newPos) { _newPos = newPos; }  // buffer _newPos
    bool advance();                                              // return true if moved

    // interaction related
    bool isPinned()  const { return _pinned; }
    bool isGrabbed() const;                // grabbed by mouse?
    void setPinned (bool pinned) { _pinned = pinned; }
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);
    virtual void     mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

    // delegated to _topology
    bool     isRoot()       const;
    int      getSize()      const;
    int      getLevel()     const;
    Node*    getParent()    const;
    NodeList getChildren()  const;
    NodeList getAncestors() const;

    // delegated to _style
    int getWidth() const;
    virtual QRectF       boundingRect() const;
    virtual QPainterPath shape()  const;
    virtual void         paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);

protected:
    void startEngine();

protected:
    NodeTopology* _topology;
    NodeStyle*    _style;
    bool    _pinned;
    QPointF _newPos;
};

} // namespace

#endif // NODE_H
