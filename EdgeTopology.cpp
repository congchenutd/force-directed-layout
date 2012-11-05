#include "EdgeTopology.h"
#include "NodeTopology.h"
#include "Node.h"

namespace ForceDirectedLayout {

EdgeTopology::EdgeTopology(NodeTopology* node1, NodeTopology* node2)
    : _node1(node1),
      _node2(node2)
{
	_node1->addEdgeTopology(this);
	_node2->addEdgeTopology(this);
}

NodeTopology* EdgeTopology::getTheOtherTopology(const NodeTopology* node) const {
    return getTopology1() == node ? getTopology2() : getTopology1();
}

Node* EdgeTopology::getNode1() const {
    return getTopology1()->getNode();
}

Node* EdgeTopology::getNode2() const {
    return getTopology2()->getNode();
}

Node *EdgeTopology::getTheOtherNode(const Node* node) const {
    return getTheOtherTopology(node->getTopology())->getNode();
}

bool EdgeTopology::isDangling() const {
    return _node1 == 0 || _node2 == 0;
}

}
