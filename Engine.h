#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>

class QGraphicsView;

namespace ForceDirectedLayout {


// Interface for layout algorithms
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


class Framer
{

};

class Node;
class Edge;
typedef QList<Node*> NodeList;

// iterative, push and pull
class IterativeEngine : public Engine
{
public:
    IterativeEngine(QGraphicsView* view);
    void setRunningRate(int rate) { _rate = rate; }
    void setSensitivity     (qreal sensitivity) { _sensitivity = sensitivity; }
    void setToughness       (qreal toughness)   { _toughness   = toughness;   }
    void setPullingAmplifier(qreal amplifier)   { _pullingAmplifier = amplifier; }
    void setPushingAmplifier(qreal amplifier)   { _pushingAmplifier = amplifier; }

    virtual void start();
    virtual void stop();
    virtual bool isRunning() const { return _timerID != 0; }

protected:
    virtual void timerEvent(QTimerEvent* event);   // faster than QTimer
    virtual bool step() = 0;                       // one iteration for one timer event, return true if moved
    virtual void calculateForces(Node* node);      // push and pull
    virtual NodeList getPushers(const Node* node) const = 0;
    virtual void push(Node* node, qreal& xMove, qreal& yMove) = 0;
    virtual void pull(Node* node, qreal& dx, qreal& dy);

protected:
    QGraphicsView* _view;
    int   _timerID;
    int   _rate;             // in msec
    qreal _sensitivity;
    qreal _toughness;
    qreal _pullingAmplifier;
    qreal _pushingAmplifier;
};


// all other nodes are considered as pushers
class GlobalEngine : public IterativeEngine
{
public:
    GlobalEngine(QGraphicsView* view) : IterativeEngine(view) {}

protected:
    virtual bool step();
    virtual void push(Node* node, qreal& xMove, qreal& yMove);
    virtual NodeList getPushers(const Node* node) const;
};


class View;

// A multi-scale algorithm
// only works with trees
class LocalEngine : public IterativeEngine
{
public:
    LocalEngine(View* view);

protected:
    virtual bool step();
    virtual void push(Node* node, qreal& xMove, qreal& yMove);
    virtual NodeList getPushers(const Node* node) const;

private:
    View* _view;
};

}


#endif // ENGINE_H
