//
// Created by giacomo on 24/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_PETRINET_H
#define FUZZYSTRINGMATCHING2_PETRINET_H

#include <ostream>

template <typename EdgeLabelType, typename VertexIdType>
struct HalfOfEdge {
    EdgeLabelType edgeLabel;
    VertexIdType  adjacentVertexId;

    HalfOfEdge() = default;
    HalfOfEdge(const HalfOfEdge&) = default;
    HalfOfEdge& operator=(const HalfOfEdge&) = default;
    HalfOfEdge(EdgeLabelType edgeLabel, VertexIdType adjacentVertexId) : edgeLabel(edgeLabel),
                                                                         adjacentVertexId(adjacentVertexId) {}

    friend std::ostream &operator<<(std::ostream &os, const HalfOfEdge &edge) {
        return os << "{" << edge.edgeLabel << "," << edge.adjacentVertexId << "}";
    }

    bool operator==(const HalfOfEdge &rhs) const {
        return edgeLabel == rhs.edgeLabel &&
               adjacentVertexId == rhs.adjacentVertexId;
    }
    bool operator<(const HalfOfEdge &rhs) const {
        if (edgeLabel < rhs.edgeLabel) return true;
        if (rhs.edgeLabel < edgeLabel) return false;
        return adjacentVertexId < rhs.adjacentVertexId;
    }
    bool operator>(const HalfOfEdge &rhs) const { return rhs < *this; }
    bool operator<=(const HalfOfEdge &rhs) const { return !(rhs < *this); }
    bool operator>=(const HalfOfEdge &rhs) const { return !(*this < rhs); }
    bool operator!=(const HalfOfEdge &rhs) const { return !(rhs == *this); }
};

#include <utils/hash_combine.h>
#include <vector>

namespace std {
    template <typename T>
    struct hash<std::vector<T>> {
        std::size_t operator()(const std::vector<size_t>& k) const {
            size_t init = 31;
            std::hash<T> thash;
            for (const auto& x : k) init = hash_combine(init, thash(x));
            return init;
        }
    };
}

namespace std {
    template <typename EdgeLabelType, typename VertexIdType>
    struct hash<HalfOfEdge<EdgeLabelType,VertexIdType>> {
        std::size_t operator()(const HalfOfEdge<EdgeLabelType,VertexIdType>& k) const {
            std::hash<EdgeLabelType> edgeHasher;
            std::hash<VertexIdType> vertexHasher;
            size_t init = 31;
            return hash_combine(hash_combine(31, edgeHasher(k.edgeLabel)), vertexHasher(k.adjacentVertexId));
        }
    };
}

#include <vector>
#include <map>
#include <unordered_map>
#include <cassert>
#include <set>
#include <unordered_set>
#include <queue>
#include <utils/fixed_bimap.h>
#include <boost/unordered_map.hpp>
#include <Eigen/Dense>

/*namespace std
{
    template<> struct less<Eigen::VectorXd>
    {
        bool operator() (const Eigen::VectorXd& __x, const Eigen::VectorXd& __y) const
        {
            return std::lexicographical_compare(__x.begin(), __x.end(),
                                                __y.begin(), __y.end());
        }
    };
}*/

bool
operator<(const Eigen::VectorXd& __x, const Eigen::VectorXd& __y);

std::ostream& operator<< (std::ostream& out, const  std::vector<size_t> & v);



struct PetriNet {
    using Marking = std::vector<size_t>;
    using ReachabilityGraphEdge = std::pair<Marking, HalfOfEdge<size_t, Marking>>;  // .first-[.second.first]->.second.second
    using Transition_to_Marking = std::unordered_map<size_t, Marking>;



    struct ReachabilityGraph {
        Marking                                      initialState;
        std::set<Marking>                            allStates;
        std::map<Marking, std::unordered_set<HalfOfEdge<size_t, Marking>>> outgoingEdges;

        ReachabilityGraph() = default;
        ReachabilityGraph(const ReachabilityGraph&) = default;
        ReachabilityGraph& operator=(const ReachabilityGraph&) = default;

        void check() {
            size_t countInitial = 0;
            for (const auto& u : allStates) {}
        }

        std::ostream & print(std::ostream &os, const std::unordered_map<size_t, std::string>& tl);
    };

    struct MetaReachabilityGraph {
        size_t initialEState;
        size_t finalEState;
        bool isFinalEStateAddedAfterwards, isInitialEStateAddedAfterwards;
        std::unordered_map<size_t, std::unordered_set<HalfOfEdge<Marking, size_t>>> outgoingEdges;
        fixed_bimap<size_t, PetriNet::ReachabilityGraphEdge> node_id_assoc;

        MetaReachabilityGraph() = default;
        MetaReachabilityGraph(const MetaReachabilityGraph&) = default;
        MetaReachabilityGraph& operator=(const MetaReachabilityGraph&) = default;

        size_t getTransitionIdAssociatedToMetaNodeId(size_t metaNodeId) const  {
            return node_id_assoc.getValue(metaNodeId).second.edgeLabel;
        }

        void  print(std::ostream &os, const std::unordered_map<size_t, std::string>& tl, size_t eSize_plusOne) {
            os << "start: " << initialEState << " end: " << finalEState << std::endl;
            for (const auto& cp : outgoingEdges) {
                std::string repr1 = "{}", repr2 = "{}";
                if ((cp.first != 0) && ((cp.first != eSize_plusOne))) {
                    std::stringstream sst;
                    sst << tl.at( node_id_assoc.getValue(cp.first).second.edgeLabel );
                    repr1 = sst.str();
                }
                for (const auto& e: cp.second) {
                    if ((e.adjacentVertexId != 0) && (e.adjacentVertexId != eSize_plusOne)) {
                        std::stringstream sst;
                        sst << tl.at(  node_id_assoc.getValue(e.adjacentVertexId).second.edgeLabel );
                        repr2 = sst.str();
                    }
                    os << cp.first << '{' << repr1 << "}--[" << e.edgeLabel << "]-->" << repr2 << '{' << e.adjacentVertexId << '}' << std::endl;
                }
            }
        }
    };

    std::unordered_set<size_t> places, transitions;
    std::unordered_map<size_t, double> firing_weight;           // W : T -> \mathbb{R}
    std::unordered_map<size_t, std::unordered_map<size_t, size_t>> omega; // firing weight
    //std::map<std::pair<size_t,size_t>, size_t>  omega; // firing weight
    std::unordered_map<size_t, std::string> transition_labelling;
    ///boost::unordered_map<size_t, Marking>  transitionToMarking;

    size_t w_cost(size_t p, size_t t) ;

    void w_set_cost(size_t p, size_t t, size_t cost);

    bool is_transition_enabled(size_t transition_id, const Marking& marking) ;

    std::unordered_set<size_t> enabling_transitions(const Marking& marking) ;

    Transition_to_Marking generateOutgoingForReachabilityGraph(const Marking& marking) ;

    /**
     * Generates the reachability graph for the given PetriNet for a given marking (initial state, m0)
     * @param m0 Initial marking
     * @return   Resulting Reachability graph
     */
    ReachabilityGraph generateReachabilityGraph(const Marking& m0);

    /**
     * Generates the Transition Graph given the Petri Net
     * @param m0    Initial state
     * @return      Transition Graph
     */
    PetriNet::MetaReachabilityGraph generateMetaReachabilityGraph(const Marking& m0);
};


std::ostream& operator<< (std::ostream& out, const PetriNet::ReachabilityGraphEdge& e);
std::ostream& operator<< (std::ostream& out, const PetriNet::Transition_to_Marking & v);

bool doAddIState(const PetriNet::ReachabilityGraph& rg, const PetriNet::Marking& m0);
std::set<PetriNet::ReachabilityGraphEdge> getFinalState(const PetriNet::ReachabilityGraph& rg);
bool doAddFState(const PetriNet::ReachabilityGraph& rg, const PetriNet::Marking& f0);

#endif //FUZZYSTRINGMATCHING2_PETRINET_H
