#include "NodeTopology.h"
#include "EdgeTopology.h"

namespace ForceDirectedLayout {

NodeTopology::NodeTopology()
    : _node(0) {}

void NodeTopology::addEdgeTopology(EdgeTopology* edgeTopo) {
    _edgeTopoList << edgeTopo;
}

void NodeTopology::removeEdge(EdgeTopology* edgeTopo) {
    _edgeTopoList.removeOne(edgeTopo);
}

QList<Edge*> NodeTopology::getEdges() const
{
    QList<Edge*> result;
    foreach(EdgeTopology* edgeTopo, _edgeTopoList)
        result << edgeTopo->getEdge();
    return result;
}

}
