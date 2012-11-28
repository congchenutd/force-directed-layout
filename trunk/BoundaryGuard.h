#ifndef BOUNDARYGUARD_H
#define BOUNDARYGUARD_H

#include <QGraphicsRectItem>

namespace ForceDirectedLayout
{

// a rectangular boundary
class Boundary : public QGraphicsRectItem
{
public:
    bool isEscaping(const QLineF&  path)  const;
    bool isInside  (const QPointF& point) const;
    QList<QLineF> getEdges() const;
    qreal getAspectRatio() const;

    static bool isInside(const QLineF& edge, const QPointF& point);
};


class Node;

// interface for boundary constraints
class BoundaryGuard
{
public:
    BoundaryGuard(Boundary* boundary = 0);
    Boundary* getBoundary() const { return _boundary; }
    void guard(Node* node, qreal& xMove, qreal& yMove);

protected:
    // how does edge block the escaping path
    virtual void adjustEscapingPath(const QLineF& edge, QLineF& escapingPath) {}

private:
    Boundary* _boundary;
};

class NullBoundaryGuard : public BoundaryGuard
{};

// the component perpendicular to the escaped edge will be blocked
// the other component is unaffected
class AdhesiveBoundaryGuard : public BoundaryGuard
{
public:
    AdhesiveBoundaryGuard(Boundary* boundary);

protected:
    virtual void adjustEscapingPath(const QLineF& edge, QLineF& escapingPath);
};

}  // namespace

#endif // BOUNDARYGUARD_H
