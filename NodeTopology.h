#ifndef NODETOPOLOGY_H
#define NODETOPOLOGY_H

#include <QList>

namespace ForceDirectedLayout {

class EdgeTopology;
class Edge;
class Node;

// The connectivity of a node
// associates with EdgeTopology
class NodeTopology
{
public:
    typedef QList<EdgeTopology*> EdgeTopoList;

public:
    NodeTopology();
    void  setNode(Node* node) { _node = node; }
	Node* getNode() const     { return _node; }
    QList<Edge*> getEdges() const;

    virtual ~NodeTopology() {}
	virtual void addEdgeTopology   (EdgeTopology* edgeTopo);
    virtual void removeEdge(EdgeTopology* edgeTopo);

protected:
    Node*        _node;
    EdgeTopoList _edgeTopoList;
};

}

#endif // NODETOPOLOGY_H
