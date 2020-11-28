//
// Created by giacomo on 24/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_PETRINET_H
#define FUZZYSTRINGMATCHING2_PETRINET_H

#include <petrinet/HalfOfEdge.h>
#include <petrinet/RGEdge.h>

#include <vector>
#include <map>
#include <unordered_map>
#include <cassert>
#include <set>
#include <unordered_set>
#include <queue>
#include <boost/unordered_map.hpp>
#include <Eigen/Dense>
#include <petrinet/pnetUtils.h>
#include "petrinet/ReachabilityGraph.h"
#include "petrinet/MetaReachabilityGraph.h"

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




struct PetriNet {
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
    MetaReachabilityGraph generateMetaReachabilityGraph(const Marking& m0);
};


std::ostream& operator<< (std::ostream& out, const RGEdge<size_t, Marking>& e);
std::ostream& operator<< (std::ostream& out, const Transition_to_Marking & v);

bool doAddIState(const ReachabilityGraph& rg, const Marking& m0);
std::set<RGEdge<size_t, Marking>> getFinalState(const ReachabilityGraph& rg);
bool doAddFState(const ReachabilityGraph& rg, const Marking& f0);

#endif //FUZZYSTRINGMATCHING2_PETRINET_H
