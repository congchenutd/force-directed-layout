#include "BoundaryGuard.h"
#include "Node.h"
#include <QGraphicsItem>

namespace ForceDirectedLayout
{

bool Boundary::isEscaping(const QLineF& path) const {
    return isInside(path.p1()) && !isInside(path.p2());  // one in, one out
}

bool Boundary::isInside(const QPointF& point) const {
    return contains(point);
}

QList<QLineF> Boundary::getEdges() const
{
    QList<QLineF> edges;    // edges (vertices) are added counter-clockwise
    edges << QLineF(rect().topRight(),    rect().topLeft())
          << QLineF(rect().topLeft(),     rect().bottomLeft())
          << QLineF(rect().bottomLeft(),  rect().bottomRight())
          << QLineF(rect().bottomRight(), rect().topRight());
    return edges;
}

qreal Boundary::getAspectRatio() const {
    return rect().height() == 0 ? 1.0
                                : rect().width() / rect().height();
}

bool Boundary::isInside(const QLineF& edge, const QPointF& point)
{
    QLineF edgeStart2Point(edge.p1(), point);   // a line from start of the edge to point
    return edge.angleTo(edgeStart2Point) <= 180;
}

/////////////////////////////////////////////////////////////////////////////
BoundaryGuard::BoundaryGuard(Boundary* boundary)
    : _boundary(boundary)
{}

void BoundaryGuard::guard(Node* node, qreal& xMove, qreal& yMove)
{
    if(node == 0 || _boundary == 0)
        return;

    QLineF escapingPath(node->pos(), node->pos() + QPointF(xMove, yMove));
    if(!_boundary->isEscaping(escapingPath))
        return;

    foreach(const QLineF& edge, _boundary->getEdges())     // for each edge
        if(!_boundary->isInside(edge, escapingPath.p2()))  // if escape from this edge
            adjustEscapingPath(edge, escapingPath);        // block the path

    xMove = escapingPath.x2() - escapingPath.x1();
    yMove = escapingPath.y2() - escapingPath.y1();
}

/////////////////////////////////////////////////////////////////////////////
AdhesiveBoundaryGuard::AdhesiveBoundaryGuard(Boundary* boundary)
    : BoundaryGuard(boundary)
{}

void AdhesiveBoundaryGuard::adjustEscapingPath(const QLineF& edge, QLineF& escapingPath)
{
    if(edge.x1() == edge.x2())   // left or right edge
        escapingPath.setP2(QPointF(edge.x1(), escapingPath.y2()));  // use edge's x, path's y
    else                         // bottom or top
        escapingPath.setP2(QPointF(escapingPath.x2(), edge.y1()));  // path's x, edge's y
}


} // namespace
