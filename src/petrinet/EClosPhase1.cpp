//
// Created by giacomo on 28/11/20.
//

#include "petrinet/EClosPhase1.h"

EClosPhase1::EClosPhase1(PetriNet *lowLevel, ReachabilityGraph *rg, const std::string& epsilon)
        : lowLevel(lowLevel), rg(rg), epsilon(epsilon) {}

double EClosPhase1::getFiringCost(size_t transitionId) const {
    auto cost = lowLevel->firing_weight.find(transitionId);
    return cost !=  lowLevel->firing_weight.end() ? cost->second : 1.0;
}

bool EClosPhase1::getNormalizingFactor(const Marking &mark) const {
    auto it = rg->outgoingEdges.find(mark);
    if (it == rg->outgoingEdges.end()) return 0.0;
    double toRet = 0.0;
    for (const auto& t : it->second) {
        toRet += getFiringCost(t.edgeLabel);
    }
    return toRet;
}

bool EClosPhase1::isEpsilon(size_t transitionId) const {
    return lowLevel->transition_labelling.at(transitionId) == epsilon;
}

bool EClosPhase1::hasBeenVisited(const RGEdge<size_t, Marking> &edge) const {
    return map.find(edge)!= map.end();
}


void EClosPhase1::eClos(const RGEdge<size_t, Marking> &e1) {
    if (!isEpsilon(e1.edgeTarget.edgeLabel)) return;
    if (hasBeenVisited(e1)) return;
    double normalizingCost = getNormalizingFactor(e1.edgeTarget.adjacentVertexId);
    auto it = rg->outgoingEdges.find(e1.edgeTarget.adjacentVertexId);
    if (it != rg->outgoingEdges.end()) {
        RGEdge<size_t, Marking> e2;
        e2.source = e1.edgeTarget.adjacentVertexId;
        for (const HalfOfEdge<size_t, Marking>& clos : it->second) {
            e2.edgeTarget = clos;
            double p = ((normalizingCost != 0.0) ? getFiringCost(clos.edgeLabel) / normalizingCost : 1.0);
            //e2 is fully defined
            if (isEpsilon(clos.edgeLabel) && (!hasBeenVisited(e2))) {
                eClos(e2);
            }
            if (!isEpsilon(clos.edgeLabel)) {
                // I add it directly to the map if and only if such edge is not e-labelled
                map[e1].emplace_back(e2, p);
            } else {
                // If it is e-labelled, I only inherit its information.
                for (const RGEdgeWithCost<size_t, Marking>& costPair : map[e2]) {
                    map[e1].emplace_back(costPair.edge, costPair.cost * p);
                }
            }
        }
    } else {
        map[e1] = {}; // setting it as visited, but with an empty set of visited elements
    }
}
