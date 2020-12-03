//
// Created by giacomo on 28/11/20.
//

#include "petrinet/ReachabilityGraph.h"

#include <iterator> // needed for std::ostram_iterator
std::ostream &operator<<(std::ostream &out, const  std::vector<size_t> &v) {
    out << '(';
    if ( !v.empty() ) {
        std::copy (v.begin(), v.end(), std::ostream_iterator<size_t>(out, ","));
    }
    out << ")";
    return out;
}

std::ostream &ReachabilityGraph::print(std::ostream &os, const std::unordered_map<size_t, std::string> &tl) {
    for (const auto& it : outgoingEdges) {
        for (const auto &it2 : it.second) {
            os << it.first << "-[" << tl.at(it2.edgeLabel) << "]->" << it2.adjacentVertexId << std::endl;
        }
    }
    return os;
}
