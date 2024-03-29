#include "Engine.h"
#include "Edge.h"
#include "Node.h"
#include "View.h"
#include "BoundaryGuard.h"
#include <math.h>
#include <QGraphicsScene>
#include <QQueue>

namespace ForceDirectedLayout {

Engine* Engine::_currentEngine = 0;

void Engine::setCurrent(Engine* engine) {
    _currentEngine = engine;
}

Engine* Engine::getCurrent() {
    return _currentEngine;
}

//////////////////////////////////////////////////////////////////
IterativeEngine::IterativeEngine(QGraphicsView* view)
    : _view(view),
      _timerID(0),
      _rate(10),
      _sensitivity(1.0),
      _toughness(1.0),
      _pullingAmplifier(1.0),
      _pushingAmplifier(1.0),
      _boundaryGuard(new NullBoundaryGuard)
{}

qreal IterativeEngine::getDistortion() const {
//    return _boundaryGuard == 0
//            ? 1.0
//            : _boundaryGuard->getBoundary() == 0
//              ? 1.0
//              : _boundaryGuard->getBoundary()->getAspectRatio();
    return 1.0;
}

void IterativeEngine::start() {
    if(_timerID == 0)
        _timerID = startTimer(_rate);
}

void IterativeEngine::stop()
{
    killTimer(_timerID);
    _timerID = 0;
}

void IterativeEngine::timerEvent(QTimerEvent*) {
    if(!step())  // one iteration
        stop();  // stop if converged
}

void IterativeEngine::calculateForces(Node* node)
{
    if(node == 0)
        return;

    // ignore grabbed and pinned node
    if(node->isGrabbed() || node->isPinned())
    {
        node->setNewPos(node->pos());
        return;
    }

    // calculate movement by pulling and pushing
    QPointF pushForce = push(node);
    QPointF pullForce = pull(node);

    // normalize amplifiers, and calculate combined movement
    qreal xMove = pushForce.x() * _pushingAmplifier / _pullingAmplifier - pullForce.x();
    qreal yMove = pushForce.y() * _pushingAmplifier / _pullingAmplifier - pullForce.y();

    _boundaryGuard->guard(node, xMove, yMove);

    // apply movement
    if(qAbs(xMove) > _sensitivity || qAbs(yMove) > _sensitivity)   // ignore slight movement
        node->setNewPos(node->pos() + QPointF(xMove, yMove) * _toughness);
}

QPointF IterativeEngine::pull(Node* node) const
{
    if(node == 0)
        return QPointF();

    qreal xMove = 0.0;
    qreal yMove = 0.0;
    foreach(Edge* edge, node->getEdges())
    {
        Node* puller = edge->getTheOtherNode(node);
        if(puller->getLevel() <= node->getLevel())        // low level cannot pull high level
        {
            QPointF vec = node->mapToItem(puller, 0, 0);  // vector
            xMove = vec.x();
            yMove = vec.y();
        }
    }
    return QPointF(xMove, yMove);
}

QPointF IterativeEngine::push(Node* node) const
{
    if(node == 0)
        return QPointF();

    qreal xMove = 0.0;
    qreal yMove = 0.0;
    foreach(Node* pusher, getPushers(node))
    {
        QPointF force = push(node, pusher);
        xMove += force.x();
        yMove += force.y();
    }
    return QPointF(xMove * getDistortion(), yMove);
}

///////////////////////////////////////////////////////////////////////////
bool GlobalEngine::step()
{
    bool itemsMoved = false;
    foreach(QGraphicsItem* item, _view->items())     // consider all nodes
        if(Node* node = dynamic_cast<Node*>(item))
        {
            calculateForces(node);
            if(node->advance())
                itemsMoved = true;
        }
    return itemsMoved;
}

NodeList GlobalEngine::getPushers(const Node* node) const
{
    NodeList result;
    if(node == 0)
        return result;

    // all no-lower nodes are pushers
    foreach(QGraphicsItem* item, _view->items())
        if(Node* other = dynamic_cast<Node*>(item))
            if(other != node && other->getLevel() <= node->getLevel())
                result << other;
    return result;
}

QPointF GlobalEngine::push(Node* node, Node* pusher) const
{
    if(node == 0 || pusher == 0)
        return QPointF();

    QPointF vec = node->mapToItem(pusher, 0, 0);
    qreal dx = vec.x();
    qreal dy = vec.y();

    qreal distance = (dx * dx + dy * dy);
    if(distance > 0)
    {
        qreal xMove = dx / distance;
        qreal yMove = dy / distance;
        return QPointF(xMove, yMove);
    }
    return QPointF();
}


////////////////////////////////////////////////////////////////////////////
LocalEngine::LocalEngine(View *view)
    : IterativeEngine(view), _view(view)
{}

bool LocalEngine::step()
{
    Node* root = _view->getRoot();
    if(root == 0)
        return false;

    bool itemsMoved = false;
    QQueue<Node*> queue;                    // BFS
    queue.enqueue(root);
    while(!queue.isEmpty())
    {
        Node* node = queue.dequeue();

        calculateForces(node);    // calculate
        if(node->advance())       // move
            itemsMoved = true;

        queue << node->getChildren();
    }
    return itemsMoved;
}

NodeList LocalEngine::getPushers(const Node* node) const
{
    NodeList result;
    if(node == 0)
        return result;

    if(!node->isRoot())               // siblings
        foreach(Node* child, node->getParent()->getChildren())
            if(child != node)
                result << child;
    result << node->getAncestors();   // plus ancestors
    return result;
}

QPointF LocalEngine::push(Node* node, Node* pusher) const
{
    if(node == 0 || pusher == 0)
        return QPointF();

    QPointF vec = node->mapToItem(pusher, 0, 0);
    qreal dx = vec.x() == 0.0 ? qrand() / node->getWidth()   // avoid overlapping
                              : vec.x();
    qreal dy = vec.y() == 0.0 ? qrand() / node->getWidth()
                              : vec.y();

    qreal distance = sqrt(dx * dx + dy * dy);
    if(distance > 0)
    {
        qreal pushForce = sqrt(pusher->getSize() * node->getSize());
        if(pusher->getLevel() < node->getLevel())   // ancester
            pushForce *= 2;
        else                                        // sibling
            pushForce *= 1;

        qreal adjustedDistance = qMax((qreal)abs(distance - node->getWidth()/2),
                                      (qreal)node->getWidth()/2);   // avoid overlapping
        qreal xMove = pushForce * dx / adjustedDistance / adjustedDistance;
        qreal yMove = pushForce * dy / adjustedDistance / adjustedDistance;
        return QPointF(xMove, yMove);
    }
    return QPointF();
}


} // namespace
