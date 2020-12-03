//
// Created by giacomo on 28/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_REACHABILITYGRAPH_H
#define FUZZYSTRINGMATCHING2_REACHABILITYGRAPH_H

#include <petrinet/pnetUtils.h>
#include <set>
#include <unordered_set>
#include <petrinet/HalfOfEdge.h>
#include <map>


struct ReachabilityGraph {
    Marking                                      initialState;
    std::set<Marking>                            allStates;
    std::map<Marking, std::unordered_set<HalfOfEdge<size_t, Marking>>> outgoingEdges;
    //using Edge = std::pair<Marking, HalfOfEdge<size_t, Marking>>;  // .first-[.second.first]->.second.second

    ReachabilityGraph() = default;
    ReachabilityGraph(const ReachabilityGraph&) = default;
    ReachabilityGraph& operator=(const ReachabilityGraph&) = default;

    void check() {
        size_t countInitial = 0;
        for (const auto& u : allStates) {}
    }

    std::ostream & print(std::ostream &os, const std::unordered_map<size_t, std::string>& tl);
};


#endif //FUZZYSTRINGMATCHING2_REACHABILITYGRAPH_H
