#include "Engine.h"
#include "Edge.h"
#include "Node.h"
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
IterativeEngine::IterativeEngine(QGraphicsScene* scene)
    : _scene(scene),
      _rate(10),
      _sensitivity(0.1),
      _timerID(0)
{}

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

void IterativeEngine::pull(Node* node, qreal& xMove, qreal& yMove)
{
    foreach(Edge* edge, node->getEdges())
    {
        QPointF vec = node->mapToItem(edge->getTheOtherNode(node), 0, 0);  // vector
        xMove -= edge->getStrength() * vec.x();
        yMove -= edge->getStrength() * vec.y();
    }
}

void IterativeEngine::calculateForces(Node* node)
{
    // ignore grabbed and pinned node
    if(node->isGrabbed() || node->isPinned())
    {
        node->setNewPos(node->pos());
        return;
    }

    qreal xMove = 0;
    qreal yMove = 0;
    push(node, xMove, yMove);
    pull(node, xMove, yMove);

    // ignore slight move
    if(qAbs(xMove) < _sensitivity && qAbs(yMove) < _sensitivity)
        xMove = yMove = 0;
    node->setNewPos(node->pos() + QPointF(xMove, yMove));
}

///////////////////////////////////////////////////////////////////////////
bool GlobalEngine::step()
{
    bool itemsMoved = false;
    foreach(QGraphicsItem* item, _scene->items())
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
    foreach(QGraphicsItem* item, _scene->items())
        if(Node* other = dynamic_cast<Node*>(item))
            if(other != node)
                result << other;
    return result;
}

void GlobalEngine::push(Node* node, qreal& xMove, qreal& yMove)
{
    QList<Node*> pushers = getPushers(node);
    foreach(Node* pusher, pushers)
    {
        QPointF vec = node->mapToItem(pusher, 0, 0);
        qreal dx = vec.x();
        qreal dy = vec.y();

        double d = sqrt(dx * dx + dy * dy) / 10;
        if(d > 0)
        {
            xMove += dx / d;
            yMove += dy / d;
        }
    }
}


////////////////////////////////////////////////////////////////////////////
bool LocalEngine::step()
{
    bool itemsMoved = false;
    QQueue<Node*> queue;                    // BFS
//    queue.enqueue(view->getRoot());
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

void LocalEngine::push(Node* node, qreal& xMove, qreal& yMove)
{
    QList<Node*> pushers = getPushers(node);
    foreach(Node* pusher, pushers)
    {
        QPointF vec = node->mapToItem(pusher, 0, 0);
        qreal dx = vec.x() == 0.0 ? qrand() / node->getWidth()   // avoid overlapping
                                  : vec.x();
        qreal dy = vec.y() == 0.0 ? qrand() / node->getWidth()
                                  : vec.y();

        double d = sqrt(dx * dx + dy * dy);
        if(d > 0)
        {
            double pushForce = 15*sqrt(pusher->getSize() * node->getSize());
            if(pusher->getLevel() < node->getLevel())   // ancester
                pushForce *= 2;
            else                                        // sibling
                pushForce *= 1;

            double dd = qMax((double)abs(d - node->getWidth()), (double)node->getWidth());
            xMove += pushForce * dx / dd / dd;
            yMove += pushForce * dy / dd / dd;
        }
    }
}

NodeList LocalEngine::getPushers(const Node* node) const
{
    NodeList result;
    if(!node->isRoot())               // siblings
    {
        NodeList children = node->getParent()->getChildren();
        foreach(Node* child, children)
            if(child != node)
                result << child;
    }
    result << node->getAncestors();   // plus ancestors
    return result;
}


}
