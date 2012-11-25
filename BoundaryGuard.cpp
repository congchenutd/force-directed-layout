#include "BoundaryGuard.h"
#include "Edge.h"
#include "Node.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <QGraphicsItem>

namespace ForceDirectedLayout
{

bool Boundary::isEscaping(const QLineF& path) {
    return isInside(path.p1()) && !isInside(path.p2());
}

//////////////////////////////////////////////////////////////////////////////////////////
QPointF PolygonalBoundary::getStopPoint(const QLineF& escapingPath) const
{
    foreach(const QLineF& edge, getEdges())
    {
        QPointF crossing;
        if(edge.intersect(escapingPath, &crossing) == QLineF::BoundedIntersection)
            return getStopPoint(edge, escapingPath);
    }
    return escapingPath.p2();
}

QPointF PolygonalBoundary::getStopPoint(const QLineF& edge, const QLineF& escapingPath) const
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
        return escapingPath.p2();

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

    return QPointF(x, y);
}

qreal PolygonalBoundary::mod2PI(qreal angle) const
{
    qreal result = angle;
    while(result >= 360)
        result -= 360;
    return result;
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


//QPointF RectangularBoundary::getCrossing(const QLineF& path) const
//{
//    QList<QLineF> edges;
//    edges << QLineF(rect().topRight(),    rect().topLeft())
//          << QLineF(rect().topLeft(),     rect().bottomLeft())
//          << QLineF(rect().bottomLeft(),  rect().bottomRight())
//          << QLineF(rect().bottomRight(), rect().topRight());

//    foreach(const QLineF& edge, edges)
//    {
//        QPointF crossing;
//        if(edge.intersect(path, &crossing) == QLineF::BoundedIntersection)
//            return crossing;
//    }

//    return QPointF(INT_MAX, INT_MAX);
//}


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

//    foreach(QGraphicsItem* item, _view->items())
//        if(FrameEdge* frameEdge = dynamic_cast<FrameEdge*>(item))  // an edge
//        {
//            QPointF oldPos = node->pos();
//            QPointF newPos = oldPos + QPointF(xMove, yMove);
//            if(frameEdge->isInside(oldPos) && !frameEdge->isInside(newPos))  // moving out
//            {
//                QPointF vec = frameEdge->vectorFrom(node);
//                qreal dx = vec.x();
//                qreal dy = vec.y();
//                if(dx != 0)        // left or right edge
//                    xMove = dx;    // movement on y is ignored
//                if(dy != 0)        // top or bottom edge
//                    yMove = dy;    // movement on x is ignored
//            }
//        }

    QLineF path(node->pos(), node->pos() + QPointF(xMove, yMove));
    if(_boundary->isEscaping(path))
    {
//        QPointF crossing = _boundary->getCrossing(path);
//        qreal dx = crossing.x() - node->x();
//        qreal dy = crossing.y() - node->y();
//        xMove = dx;
//        yMove = dy;
        QPointF stop = _boundary->getStopPoint(path);
        xMove = stop.x() - node->x();
        yMove = stop.y() - node->y();
    }
}

} // namespace
