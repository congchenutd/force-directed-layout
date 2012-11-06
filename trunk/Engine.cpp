#include "Engine.h"
#include "Edge.h"
#include "Node.h"
#include "View.h"
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
      _rate(10),
      _sensitivity(1),
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

    qreal xMove = 0;
    qreal yMove = 0;
    push(node, xMove, yMove);
    pull(node, xMove, yMove);

    // ignore slight move
    if(qAbs(xMove) < _sensitivity && qAbs(yMove) < _sensitivity)
        xMove = yMove = 0;
    if(xMove != 0 || yMove != 0)
        node->setNewPos(node->pos() + QPointF(xMove, yMove));
}

void IterativeEngine::pull(Node* node, qreal& xMove, qreal& yMove)
{
    if(node == 0)
        return;

    foreach(Edge* edge, node->getEdges())
    {
        Node* puller = edge->getTheOtherNode(node);
        if(puller->getLevel() <= node->getLevel())
        {
            QPointF vec = node->mapToItem(edge->getTheOtherNode(node), 0, 0);  // vector
            xMove -= edge->getStrength() * vec.x();
            yMove -= edge->getStrength() * vec.y();
        }
    }
}


///////////////////////////////////////////////////////////////////////////
bool GlobalEngine::step()
{
    bool itemsMoved = false;
    foreach(QGraphicsItem* item, _view->items())
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

    foreach(QGraphicsItem* item, _view->items())
        if(Node* other = dynamic_cast<Node*>(item))
            if(other != node && other->getLevel() <= node->getLevel())
                result << other;
    return result;
}

void GlobalEngine::push(Node* node, qreal& xMove, qreal& yMove)
{
    if(node == 0)
        return;

    foreach(Node* pusher, getPushers(node))
    {
        QPointF vec = node->mapToItem(pusher, 0, 0);
        qreal dx = vec.x();
        qreal dy = vec.y();

        qreal distance = sqrt(dx * dx + dy * dy);
        qreal amplifier = 10;
        if(distance > 0)
        {
            xMove += amplifier * dx / distance;
            yMove += amplifier * dy / distance;
        }
    }
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

void LocalEngine::push(Node* node, qreal& xMove, qreal& yMove)
{
    if(node == 0)
        return;

    foreach(Node* pusher, getPushers(node))
    {
        QPointF vec = node->mapToItem(pusher, 0, 0);
        qreal dx = vec.x() == 0.0 ? qrand() / node->getWidth()   // avoid overlapping
                                  : vec.x();
        qreal dy = vec.y() == 0.0 ? qrand() / node->getWidth()
                                  : vec.y();

        qreal distance = sqrt(dx * dx + dy * dy);
        qreal amplifier = 5;
        if(distance > 0)
        {
            qreal pushForce = sqrt(pusher->getSize() * node->getSize()) * amplifier;
            if(pusher->getLevel() < node->getLevel())   // ancester
                pushForce *= 2;
            else                                        // sibling
                pushForce *= 1;

            qreal adjustedDistance = qMax((qreal)abs(distance - node->getWidth()/2),
                            (qreal)node->getWidth()/2);
            xMove += pushForce * dx / adjustedDistance;
            yMove += pushForce * dy / adjustedDistance;
        }
    }
}

NodeList LocalEngine::getPushers(const Node* node) const
{
    NodeList result;
    if(node == 0)
        return result;

    if(!node->isRoot()) {               // siblings
        foreach(Node* child, node->getParent()->getChildren())
            if(child != node)
                result << child;
    }
    result << node->getAncestors();   // plus ancestors
    return result;
}


}
