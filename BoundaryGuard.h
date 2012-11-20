#ifndef BOUNDARYGUARD_H
#define BOUNDARYGUARD_H

#include <QGraphicsView>

namespace ForceDirectedLayout
{

class Node;

// for bounding the nodes within a frame
class BoundaryGuard
{
public:
    virtual void guard(Node* node, qreal& xMove, qreal& yMove) = 0;
};

// any nodes escaping the frame will adhere to the boundaries
class AdhesiveBoundaryGuard : public BoundaryGuard
{
public:
    AdhesiveBoundaryGuard(QGraphicsView* view);
    virtual void guard(Node* node, qreal& xMove, qreal& yMove);

protected:
    QGraphicsView* _view;
};

}  // namespace

#endif // BOUNDARYGUARD_H
