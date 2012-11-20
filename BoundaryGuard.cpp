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
        if(FrameEdge* frameEdge = dynamic_cast<FrameEdge*>(item))
        {
            QPointF oldPos = node->pos();
            QPointF newPos = oldPos + QPointF(xMove, yMove);
            if(frameEdge->isInside(oldPos) && !frameEdge->isInside(newPos))
            {
                QPointF vec = frameEdge->vectorFrom(node);
                qreal dx = vec.x();
                qreal dy = vec.y();
                if(dx != 0)
                    xMove = dx;
                if(dy != 0)
                    yMove = dy;
            }
        }
}

} // namespace
