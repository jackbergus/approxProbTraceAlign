//
// Created by giacomo on 28/11/20.
//

#include "petrinet/MetaReachabilityGraph.h"

size_t MetaReachabilityGraph::getTransitionIdAssociatedToMetaNodeId(size_t metaNodeId) const {
    return node_id_assoc.getValue(metaNodeId).edgeTarget.edgeLabel;
}

void
MetaReachabilityGraph::print(std::ostream &os, const std::unordered_map<size_t, std::string> &tl, size_t eSize_plusOne) {
    os << "start: " << initialEState << " end: " << finalEState << std::endl;
    for (const auto& cp : outgoingEdges) {
        std::string repr1 = "{}", repr2 = "{}";
        if ((cp.first != 0) && ((cp.first != eSize_plusOne))) {
            std::stringstream sst;
            sst << tl.at( node_id_assoc.getValue(cp.first).edgeTarget.edgeLabel );
            repr1 = sst.str();
        }
        for (const auto& e: cp.second) {
            if ((e.adjacentVertexId != 0) && (e.adjacentVertexId != eSize_plusOne)) {
                std::stringstream sst;
                sst << tl.at(  node_id_assoc.getValue(e.adjacentVertexId).edgeTarget.edgeLabel );
                repr2 = sst.str();
            }
            os << cp.first << '{' << repr1 << "}--[" << e.edgeLabel << "]-->" << repr2 << '{' << e.adjacentVertexId << '}' << std::endl;
        }
    }
}
