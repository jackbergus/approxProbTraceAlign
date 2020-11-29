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

struct MetaReachabilityGraphHalfWeightedEdge {
    size_t dst;
    double probability;

    MetaReachabilityGraphHalfWeightedEdge(size_t dst, double probability);
    MetaReachabilityGraphHalfWeightedEdge() : MetaReachabilityGraphHalfWeightedEdge{0, 1.0} {}
    MetaReachabilityGraphHalfWeightedEdge(const MetaReachabilityGraphHalfWeightedEdge& ) = default;
    MetaReachabilityGraphHalfWeightedEdge& operator=(const MetaReachabilityGraphHalfWeightedEdge& ) = default;

    bool operator==(const MetaReachabilityGraphHalfWeightedEdge &rhs) const;
    bool operator!=(const MetaReachabilityGraphHalfWeightedEdge &rhs) const;
    bool operator<(const MetaReachabilityGraphHalfWeightedEdge &rhs) const;
    bool operator>(const MetaReachabilityGraphHalfWeightedEdge &rhs) const;
    bool operator<=(const MetaReachabilityGraphHalfWeightedEdge &rhs) const;
    bool operator>=(const MetaReachabilityGraphHalfWeightedEdge &rhs) const;
};

namespace std {
    template <>
    struct hash<MetaReachabilityGraphHalfWeightedEdge> {
        size_t operator()(const MetaReachabilityGraphHalfWeightedEdge& ref) const {
            std::hash<double> doubleHasher;
            return hash_combine(hash_combine(31, ref.dst), doubleHasher(ref.probability));
        }
    };
}

struct MetaReachabilityGraph {
    size_t initialEState;
    size_t finalEState;
    bool isFinalEStateAddedAfterwards, isInitialEStateAddedAfterwards;
    std::unordered_map<size_t, std::unordered_set</*HalfOfEdgeWithProbability<Marking, size_t>*/MetaReachabilityGraphHalfWeightedEdge>> outgoingEdges;
    fixed_bimap<size_t, RGEdge<size_t, Marking>> node_id_assoc;

    MetaReachabilityGraph() = default;
    MetaReachabilityGraph(const MetaReachabilityGraph&) = default;
    MetaReachabilityGraph& operator=(const MetaReachabilityGraph&) = default;

    size_t getTransitionIdAssociatedToMetaNodeId(size_t metaNodeId) const;
    void  print(std::ostream &os, const std::unordered_map<size_t, std::string>& tl, size_t eSize_plusOne);
};


#endif //FUZZYSTRINGMATCHING2_METAREACHABILITYGRAPH_H
