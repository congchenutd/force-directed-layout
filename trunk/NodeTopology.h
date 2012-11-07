#ifndef NODETOPOLOGY_H
#define NODETOPOLOGY_H

#include <QList>

namespace ForceDirectedLayout {

class EdgeTopology;
class Edge;
class Node;

typedef QList<Node*> NodeList;

// The connectivity of a node
// associates with EdgeTopology
// applies to any graph node
class NodeTopology
{
public:
    NodeTopology();
    void  setNode(Node* node) { _node = node; }   // called by Node's cstr bia setTopology()
	Node* getNode() const     { return _node; }
    QList<Edge*> getEdges() const;

    virtual ~NodeTopology() {}
    virtual void addEdgeTopology   (EdgeTopology* edgeTopo);
    virtual void removeEdgeTopology(EdgeTopology* edgeTopo);

    virtual bool     isRoot()       const { return false; }
    virtual int      getSize()      const { return 1;     }
    virtual int      getLevel()     const { return -1;    }
    virtual Node*    getParent()    const { return 0;     }
    virtual NodeList getChildren()  const { return NodeList(); }
    virtual NodeList getAncestors() const { return NodeList(); }

protected:
    Node*                _node;
    QList<EdgeTopology*> _edgeTopoList;
};

class TreeNodeTopology : public NodeTopology
{
public:
    TreeNodeTopology();

    virtual bool     isRoot()       const;
    virtual int      getLevel()     const { return _level;  }
    virtual Node*    getParent()    const;
    virtual NodeList getChildren()  const;
    virtual NodeList getAncestors() const;

    virtual void addEdgeTopology   (EdgeTopology* edgeTopo);
    virtual void removeEdgeTopology(EdgeTopology* edgeTopo);

protected:
    int           _level;
    NodeTopology* _parent;
};

}

#endif // NODETOPOLOGY_H
