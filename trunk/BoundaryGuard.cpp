#include "BoundaryGuard.h"
#include "Edge.h"
#include "Node.h"
#include <QGraphicsItem>

namespace ForceDirectedLayout
{

bool Boundary::isEscaping(const QLineF& path) {
    return isInside(path.p1()) && !isInside(path.p2());
}

QPointF PolygonalBoundary::getStopPoint(const QLineF& edge, const QLineF& escapingPath) const
{

}

bool RectangularBoundary::isInside(const QPointF& point) const {
    return contains(point);
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
    }
}


} // namespace
