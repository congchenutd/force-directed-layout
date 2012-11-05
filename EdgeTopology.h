#ifndef EDGETOPOLOGY_H
#define EDGETOPOLOGY_H

namespace ForceDirectedLayout {

class Edge;
class NodeTopology;
class Node;

class EdgeTopology
{
public:
    EdgeTopology(NodeTopology* node1, NodeTopology* node2);
    void setEdge(Edge* edge) { _edge = edge; }
    Edge*         getEdge()  const { return _edge; }
    NodeTopology* getTopology1() const { return _node1; }
    NodeTopology* getTopology2() const { return _node2; }
    NodeTopology* getTheOtherTopology(const NodeTopology* node) const;
    Node* getNode1() const;
    Node* getNode2() const;
    Node* getTheOtherNode(const Node* node) const;
    bool isDangling() const;

protected:
    Edge*         _edge;
    NodeTopology* _node1;
    NodeTopology* _node2;
};

}

#endif // EDGETOPOLOGY_H
