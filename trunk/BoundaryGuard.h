#ifndef BOUNDARYGUARD_H
#define BOUNDARYGUARD_H

#include <QGraphicsView>
#include <QGraphicsRectItem>

namespace ForceDirectedLayout
{

class Node;

class Boundary
{
public:
    bool isEscaping(const QLineF& path);

    virtual bool    isInside(const QPointF& point)  const = 0;
    virtual QPointF getStopPoint(const QLineF& escapingPath) const = 0;
};

class PolygonalBoundary : public Boundary
{
public:
    virtual QPointF getStopPoint(const QLineF& escapingPath) const;

protected:
    virtual QPointF getStopPoint(const QLineF& edge, const QLineF& escapingPath) const;
    virtual QList<QLineF> getEdges() const = 0;

private:
    qreal mod2PI(qreal angle) const;
};

class TriangularBoundary : public PolygonalBoundary, public QGraphicsPolygonItem
{
public:
    virtual bool isInside(const QPointF& point) const;
    virtual QList<QLineF> getEdges() const;
};

class RectangularBoundary : public PolygonalBoundary, public QGraphicsRectItem
{
public:
    virtual bool isInside(const QPointF& point) const;
    virtual QList<QLineF> getEdges() const;
};

// interface for bounding the nodes within a frame
class BoundaryGuard
{
public:
    BoundaryGuard(Boundary* boundary = 0);
    virtual void guard(Node* node, qreal& xMove, qreal& yMove) = 0;

protected:
    Boundary* _boundary;
};

class NullBoundaryGuard : public BoundaryGuard
{
public:
    virtual void guard(Node* node, qreal& xMove, qreal& yMove) {}
};

// any nodes escaping the frame will adhere to the boundaries
class AdhesiveBoundaryGuard : public BoundaryGuard
{
public:
    AdhesiveBoundaryGuard(Boundary* boundary);
    virtual void guard(Node* node, qreal& xMove, qreal& yMove);

protected:
    QGraphicsView* _view;
};

}  // namespace

#endif // BOUNDARYGUARD_H
