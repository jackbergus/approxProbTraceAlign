//
// Created by giacomo on 28/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_ECLOSPHASE1_H
#define FUZZYSTRINGMATCHING2_ECLOSPHASE1_H


#include "PetriNet.h"



class EClosPhase1 {
    PetriNet* lowLevel;
    ReachabilityGraph* rg;
    std::string epsilon;

public:
    std::unordered_map<RGEdge<size_t, Marking>, std::vector<RGEdgeWithCost<size_t, Marking>>> map;
    EClosPhase1(PetriNet *lowLevel, ReachabilityGraph *rg, const std::string& epsilon = ".");

    double getFiringCost(size_t transitionId) const;
    bool getNormalizingFactor(const Marking& mark) const;
    bool isEpsilon(size_t transitionId) const;
    bool hasBeenVisited(const RGEdge<size_t, Marking>& edge) const;


    void eClos(const RGEdge<size_t, Marking>& e1);

};


#endif //FUZZYSTRINGMATCHING2_ECLOSPHASE1_H
