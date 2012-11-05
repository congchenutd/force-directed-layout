#include "NodeTopology.h"
#include "EdgeTopology.h"
#include "Node.h"

namespace ForceDirectedLayout {

NodeTopology::NodeTopology()
    : _node(0) {}

void NodeTopology::addEdgeTopology(EdgeTopology* edgeTopo) {
    _edgeTopoList << edgeTopo;
}

void NodeTopology::removeEdgeTopology(EdgeTopology* edgeTopo) {
    _edgeTopoList.removeOne(edgeTopo);
}

QList<Edge*> NodeTopology::getEdges() const
{
    QList<Edge*> result;
    foreach(EdgeTopology* edgeTopo, _edgeTopoList)
        result << edgeTopo->getEdge();
    return result;
}


//////////////////////////////////////////////////////////////////
TreeNodeTopology::TreeNodeTopology()
    : _level(-1), _parent(0)
{
}

bool TreeNodeTopology::isRoot() const {
    return getLevel() == 0;
}

Node *TreeNodeTopology::getParent() const {
    return _parent->getNode();
}

NodeList TreeNodeTopology::getChildren() const
{
    NodeList result;
    foreach(const EdgeTopology* edgeTopo, _edgeTopoList)
        if(edgeTopo->getTopology1() == this)
            result << edgeTopo->getNode2();
    return result;
}

NodeList TreeNodeTopology::getAncestors() const
{
    NodeList result;
    Node* parent = getParent();
    while(parent != 0)
    {
        result << parent;
        parent = parent->getTopology()->getParent();
    }
    return result;
}

void TreeNodeTopology::addEdgeTopology(EdgeTopology* edgeTopo)
{
    NodeTopology::addEdgeTopology(edgeTopo);
    if(edgeTopo->getTopology2() == this)
    {
        _parent = edgeTopo->getTopology1();
        _level = _parent == 0 ? -1 : _parent->getLevel() + 1;
    }
}

void TreeNodeTopology::removeEdgeTopology(EdgeTopology* edgeTopo)
{
    NodeTopology::removeEdgeTopology(edgeTopo);
    if(edgeTopo->getTopology2() == this)
    {
        _parent = 0;
        _level = -1;
    }
}



}
