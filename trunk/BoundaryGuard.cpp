#include "BoundaryGuard.h"
#include "Edge.h"
#include "Node.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <QGraphicsItem>
#include <QDebug>

namespace ForceDirectedLayout
{

bool Boundary::isEscaping(const QLineF& path) const {
    return isInside(path.p1()) && !isInside(path.p2());
}

//////////////////////////////////////////////////////////////////////////////////////////
void PolygonalBoundary::adjustEscapingPath(QLineF& escapingPath) const
{
    foreach(const QLineF& edge, getEdges())
    {
        if(isEscaping(edge, escapingPath))
        {
            QPointF target = escapingPath.p2();
            adjustEscapingPath(edge, escapingPath);
            QPointF result = escapingPath.p2();
//            qDebug() << "target = " << target << "stop at = " << result;
        }
    }
}

void PolygonalBoundary::adjustEscapingPath(const QLineF& edge, QLineF& escapingPath) const
{
    qreal alpha = escapingPath.angleTo(edge);
    qreal beta  = mod2PI(alpha + 270);
    qreal gamma = mod2PI(beta + 270);

    // normal component
    QLineF normalComponent = escapingPath;
    qreal normalAngle = mod2PI(escapingPath.angle() + beta);
    normalComponent.setAngle(normalAngle);

    QPointF normalCrossing;
    if(edge.intersect(normalComponent, &normalCrossing) == QLineF::NoIntersection)
        return;

    QLineF normalComponentCut(escapingPath.p1(), normalCrossing);

    // parallel component
    QLineF parallelComponent = escapingPath;
    qreal parallelAngle = mod2PI(escapingPath.angle() + gamma);
    parallelComponent.setAngle(parallelAngle);
    parallelComponent.setLength(escapingPath.length() * sin(beta/180*M_PI));

    // combine two components
    qreal x = escapingPath.p1().x();
    x += normalComponentCut.p2().x() - normalComponentCut.p1().x();
    x += parallelComponent.p2().x() - parallelComponent.p1().x();

    qreal y = escapingPath.p1().y();
    y += normalComponentCut.p2().y() - normalComponentCut.p1().y();
    y += parallelComponent.p2().y() - parallelComponent.p1().y();

    escapingPath.setP2(QPointF(x, y));
}

qreal PolygonalBoundary::mod2PI(qreal angle) const
{
    qreal result = angle;
    while(result >= 360)
        result -= 360;
    return result;
}

bool PolygonalBoundary::isEscaping(const QLineF& edge, const QLineF& path) const {
    return isInside(edge, path.p1()) && !isInside(edge, path.p2());
}

bool PolygonalBoundary::isInside(const QLineF& edge, const QPointF& point) const
{
    QLineF edgeStart2Point(edge.p1(), point);
    return edge.angleTo(edgeStart2Point) <= 180;
}

////////////////////////////////////////////////////////////////////////////////////////////
bool TriangularBoundary::isInside(const QPointF& point) const {
    return contains(point);
}

QList<QLineF> TriangularBoundary::getEdges() const
{
    QList<QLineF> result;
    QPolygonF triangle = polygon();
    if(triangle.size() != 3)
        return result;

    result << QLineF(triangle[0], triangle[1])
           << QLineF(triangle[1], triangle[2])
           << QLineF(triangle[2], triangle[0]);
    return result;
}

////////////////////////////////////////////////////////////////////////////////////////////
bool RectangularBoundary::isInside(const QPointF& point) const {
    return contains(point);
}

QList<QLineF> RectangularBoundary::getEdges() const
{
    QList<QLineF> edges;
    edges << QLineF(rect().topRight(),    rect().topLeft())
          << QLineF(rect().topLeft(),     rect().bottomLeft())
          << QLineF(rect().bottomLeft(),  rect().bottomRight())
          << QLineF(rect().bottomRight(), rect().topRight());
    return edges;
}


/////////////////////////////////////////////////////////////////////////////
BoundaryGuard::BoundaryGuard(Boundary* boundary)
    : _boundary(boundary)
{}

AdhesiveBoundaryGuard::AdhesiveBoundaryGuard(Boundary* boundary)
    : BoundaryGuard(boundary)
{}

void AdhesiveBoundaryGuard::guard(Node* node, qreal& xMove, qreal& yMove)
{
    if(_view == 0 || node == 0)
        return;

    QLineF path(node->pos(), node->pos() + QPointF(xMove, yMove));
    if(_boundary->isEscaping(path))
    {
        _boundary->adjustEscapingPath(path);
        xMove = path.x2() - path.x1();
        yMove = path.y2() - path.y1();
    }
}

} // namespace
