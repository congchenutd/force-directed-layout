#include "BoundaryGuard.h"
#include "Edge.h"
#include "Node.h"
#include <QGraphicsItem>

namespace ForceDirectedLayout
{

AdhesiveBoundaryGuard::AdhesiveBoundaryGuard(QGraphicsView* view)
    : _view(view)
{}

void AdhesiveBoundaryGuard::guard(Node* node, qreal& xMove, qreal& yMove)
{
    if(_view == 0 || node == 0)
        return;

    foreach(QGraphicsItem* item, _view->items())
        if(FrameEdge* frameEdge = dynamic_cast<FrameEdge*>(item))  // a edge
        {
            QPointF oldPos = node->pos();
            QPointF newPos = oldPos + QPointF(xMove, yMove);
            if(frameEdge->isInside(oldPos) && !frameEdge->isInside(newPos))  // moving out
            {
                QPointF vec = frameEdge->vectorFrom(node);
                qreal dx = vec.x();
                qreal dy = vec.y();
                if(dx != 0)        // left or right edge
                    xMove = dx;    // movement on y is ignored
                if(dy != 0)        // top or bottom edge
                    yMove = dy;    // movement on x is ignored
            }
        }
}

} // namespace
