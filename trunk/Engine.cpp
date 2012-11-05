#include "Engine.h"
#include "Edge.h"
#include "Node.h"
#include <math.h>
#include <QGraphicsScene>

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

QList<Node*> GlobalEngine::getPushers(const Node* node) const
{
    QList<Node*> result;
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


}
