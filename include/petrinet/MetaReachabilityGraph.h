//
// Created by giacomo on 28/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_METAREACHABILITYGRAPH_H
#define FUZZYSTRINGMATCHING2_METAREACHABILITYGRAPH_H

#include <petrinet/pnetUtils.h>
#include <petrinet/HalfOfEdge.h>
#include <unordered_set>
#include <petrinet/RGEdge.h>
#include <utils/fixed_bimap.h>

struct MetaReachabilityGraph {
    size_t initialEState;
    size_t finalEState;
    bool isFinalEStateAddedAfterwards, isInitialEStateAddedAfterwards;
    std::unordered_map<size_t, std::unordered_set<HalfOfEdge<Marking, size_t>>> outgoingEdges;
    fixed_bimap<size_t, RGEdge<size_t, Marking>> node_id_assoc;

    MetaReachabilityGraph() = default;
    MetaReachabilityGraph(const MetaReachabilityGraph&) = default;
    MetaReachabilityGraph& operator=(const MetaReachabilityGraph&) = default;

    size_t getTransitionIdAssociatedToMetaNodeId(size_t metaNodeId) const;
    void  print(std::ostream &os, const std::unordered_map<size_t, std::string>& tl, size_t eSize_plusOne);
};


#endif //FUZZYSTRINGMATCHING2_METAREACHABILITYGRAPH_H
