#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>

class QGraphicsScene;

namespace ForceDirectedLayout {


// the layout algorithm
class Engine : public QObject
{
public:
    virtual ~Engine() {}
    virtual void start() = 0;
    virtual void stop()  = 0;
    virtual bool isRunning() const = 0;

    static void setCurrent(Engine* engine);
    static Engine* getCurrent();

protected:
    static Engine* _currentEngine;
};


class Node;
typedef QList<Node*> NodeList;

// iterative, push and pull
class IterativeEngine : public Engine
{
public:
    IterativeEngine(QGraphicsScene* scene);
    void setRunningRate(int rate) { _rate = rate; }
    void setSensitivity(double sensitivity) { _sensitivity = sensitivity; }

    virtual void start();
    virtual void stop();
    virtual bool isRunning() const { return _timerID != 0; }

protected:
    virtual void timerEvent(QTimerEvent* event);   // faster than QTimer
    virtual bool step() = 0;                       // one iteration for one timer event, return true if moved
    virtual void calculateForces(Node* node);      // push and pull
    virtual NodeList getPushers(const Node* node) const = 0;
    virtual void push(Node* node, qreal& xMove, qreal& yMove) = 0;
    virtual void pull(Node* node, qreal& xMove, qreal& yMove);

protected:
    QGraphicsScene* _scene;
    int    _rate;
    double _sensitivity;
    int    _timerID;
};


// all other nodes are considered as pushers
class GlobalEngine : public IterativeEngine
{
public:
    GlobalEngine(QGraphicsScene* scene) : IterativeEngine(scene) {}

protected:
    virtual bool step();
    virtual void push(Node* node, qreal& xMove, qreal& yMove);
    virtual NodeList getPushers(const Node* node) const;
};


class LocalEngine : public IterativeEngine
{
public:
    LocalEngine(QGraphicsScene* scene) : IterativeEngine(scene) {}

protected:
    virtual bool step();
    virtual void push(Node* node, qreal& xMove, qreal& yMove);
    virtual NodeList getPushers(const Node* node) const;
};

}


#endif // ENGINE_H
