//
// Created by giacomo on 24/11/20.
//

#include "petrinet/PetriNet.h"

size_t PetriNet::w_cost(size_t p, size_t t)  {
    auto it = omega.find(p);
    if (it == omega.end()) return 0;
    auto it2 = it->second.find(t);
    return (it2 == it->second.end()) ? 0 : it2->second;
}

void PetriNet::w_set_cost(size_t p, size_t t, size_t cost) {
    omega[p][t] = cost;
}

bool PetriNet::is_transition_enabled(size_t transition_id, const Marking &marking)  {
    assert((places.size()) == marking.size());
    for (const size_t& place_id : places) {
        if (marking[(place_id)] < w_cost(place_id, transition_id)) return false;
    }
    return true;
}

std::unordered_set<size_t> PetriNet::enabling_transitions(const Marking &marking)  {
    assert((places.size()) == marking.size());
    std::unordered_set<size_t> result;
    for (const size_t& place_id : places) {
        if (marking[(place_id)] != 0.0) {
            for (const size_t& transition_id : transitions) {
                if (is_transition_enabled(transition_id, marking)) result.emplace(transition_id);
            }
        }
    }
    return result;
}

Transition_to_Marking PetriNet::generateOutgoingForReachabilityGraph(const Marking &marking)  {
    Transition_to_Marking transitionToMarking;
    for (const size_t& transition: enabling_transitions(marking)) {
        ///std::cout << '*' << transition << std::endl;
        Marking cpy = marking;
        const auto itAdd = omega.find(transition);
        if (itAdd != omega.end()) for (const auto& cp : itAdd->second) {
            ///std::cerr << cpy.size() << " " << cp.first << " " << cp.second << std::endl;
            cpy[cp.first] += cp.second;
        }
        for (const size_t& p : places) {
            ///std::cerr << cpy.size() << " " << p << std::endl;
            cpy[p] -= w_cost(p, transition);
        }
        transitionToMarking[transition] = cpy;
    }
    ///std::cout << transitionToMarking.size() << std::endl;
    return transitionToMarking;
}

ReachabilityGraph PetriNet::generateReachabilityGraph(const Marking &m0) {
    ReachabilityGraph rg;
    rg.initialState = m0;
    std::queue<Marking> q;
    q.emplace(m0);
    while (!q.empty()) {
        const Marking& m = q.front();
        if (!rg.allStates.insert(m).second) {
            q.pop();
            continue;
        } else {
            for (const auto& it : generateOutgoingForReachabilityGraph(m)) {
                rg.outgoingEdges[m].emplace(it.first, it.second);
                ///rg.outgoingEdges[m]    [it.second] = (it.first);
                q.push(it.second);
            }
            q.pop();
        }
    }

    /// Assertion zone
    std::cerr << "(performing assertion)" << std::flush;
    // 1) there should be only one vertex having no ingoing edges
    {
        std::map<Marking, std::unordered_set<HalfOfEdge<size_t, Marking>>> ingoingEdges;
        for (const auto& cp: rg.outgoingEdges) {
            for (const auto& e : cp.second) {
                ingoingEdges[e.adjacentVertexId].emplace(e.edgeLabel, cp.first);
            }
        }
        size_t toCount = 0;
        for (const auto& v : rg.allStates) {
            if (ingoingEdges.find(v) == ingoingEdges.end()) {
                toCount++;
                std::cout << v << std::endl;
            }
        }
        assert(toCount == 1);
    }

    return rg;
}

#include <fstream>
#include <petrinet/EClosPhase1.h>

MetaReachabilityGraph PetriNet::generateMetaReachabilityGraph(const Marking &m0, const std::string& epsilon) {
    std::cerr << "Generating the reachability graph... " << std::flush;
    auto rg = generateReachabilityGraph(m0);
    MetaReachabilityGraph mrg;
    size_t eSize_plusOne = 1;

    {
        EClosPhase1 phase1{this, &rg};

        /*{
            std::ofstream of{"out/01_rg.txt"};
            rg.print(of, transition_labelling);
        }*/

        std::cerr << "done!" << std::endl << "Generating the Transition Graph from it (1: vertex scan +closure part 1)... " << std::flush;

        // Defining the vertices and their labelling from the graph's nodes
        RGEdge<size_t, Marking> tmp;
        std::vector<RGEdge<size_t, Marking>> edgesToRescan;
        for (const auto& cp : rg.outgoingEdges) {
            tmp.source = cp.first;
            for (const auto& e : cp.second) {
                tmp.edgeTarget = e;
                // Adding it as a node if and only if it is a starting node, or if it is a final node, or if it is a non-epsilon node
                auto it = rg.outgoingEdges.find(tmp.edgeTarget.adjacentVertexId);
                auto label = transition_labelling.at(tmp.edgeTarget.edgeLabel);
                if (label == epsilon) {
                    phase1.eClos(tmp);
                }
                if ((tmp.source == m0) || (it == rg.outgoingEdges.end() || it->second.empty()) || (label != epsilon)) {
                    mrg.node_id_assoc.put(eSize_plusOne++, tmp);
                }
            }
        }
        std::cerr << "done!" << std::endl << "Generating the Transition Graph from it (2: edge scan, +closure part 2)... " << std::flush;

        // Defining the edges by the two-step edges
        /**for (const auto& v_out : rg.outgoingEdges) {
            tmp.source = v_out.first;
            for (const auto& e : v_out.second) {
                tmp.edgeTarget = e;*/
        for (const auto& cp_e : mrg.node_id_assoc.getElements())  {  /// <--
                const auto tmp = cp_e.second;                        /// <--
                size_t tmp_key = mrg.node_id_assoc.getKey(tmp);
                RGEdge<size_t, Marking> tmp2;
                ///tmp2.source = e.adjacentVertexId;
                tmp2.source = tmp.edgeTarget.adjacentVertexId;       /// <--  e.adjacentVertexId -> tmp.edgeTarget.adjacentVertexId
                ///const auto it = rg.outgoingEdges.find(e.adjacentVertexId);

                double normalizingCost = phase1.getNormalizingFactor(tmp2.source);
            const auto it = rg.outgoingEdges.find(tmp.edgeTarget.adjacentVertexId); /// <--
                if (it != rg.outgoingEdges.end()) {
                    for (const auto& e2 : it->second) {
                        tmp2.edgeTarget = e2;

                        auto isEdgeIt = phase1.map.find(tmp2);
                        double p = (normalizingCost != 0.0) ? phase1.getFiringCost(tmp2.edgeTarget.edgeLabel) / normalizingCost : 1.0;
                        if (isEdgeIt == phase1.map.end()) { // if tmp2 is not an e-edge, and therefore it is not in the e-map
                            HalfOfEdgeWithCost<Marking, size_t> weightedEdgeTransfer;
                            weightedEdgeTransfer.halfEdge.edgeLabel        = tmp.edgeTarget.adjacentVertexId;
                            weightedEdgeTransfer.halfEdge.adjacentVertexId = mrg.node_id_assoc.getKey(tmp2);
                            weightedEdgeTransfer.cost                      = p;
                            mrg.outgoingEdges[tmp_key].emplace_back(weightedEdgeTransfer);
                        } else {
                            // only with clos, phase1
                            for (const RGEdgeWithCost<size_t, Marking>& weightedEdge : isEdgeIt->second) {
                                HalfOfEdgeWithCost<Marking, size_t> weightedEdgeTransfer;
                                weightedEdgeTransfer.halfEdge.edgeLabel        = weightedEdge.edge.edgeTarget.adjacentVertexId;
                                weightedEdgeTransfer.halfEdge.adjacentVertexId = mrg.node_id_assoc.getKey(weightedEdge.edge);
                                weightedEdgeTransfer.cost                      = p * weightedEdge.cost;
                                mrg.outgoingEdges[tmp_key].emplace_back(weightedEdgeTransfer);
                            }
                        }

                    }
                }
        }
        /**}
}*/
        if (doAddIState(rg, m0)) { // If I need to add an explicit initial state, because there are many edges starting from the initial node
            mrg.initialEState                  = 0;
            mrg.isInitialEStateAddedAfterwards = true;
            double normalizingCost = phase1.getNormalizingFactor(m0);
            RGEdge<size_t, Marking> m0out;
            m0out.source = m0;
            for (const auto& e : rg.outgoingEdges.at(m0)) {
                m0out.edgeTarget = e;
                HalfOfEdgeWithCost<Marking, size_t> weightedEdgeTransfer;
                weightedEdgeTransfer.halfEdge.edgeLabel        = m0;
                weightedEdgeTransfer.halfEdge.adjacentVertexId = mrg.node_id_assoc.getKey(m0out);
                weightedEdgeTransfer.cost                      = (normalizingCost != 0.0) ? phase1.getFiringCost(e.edgeLabel) / normalizingCost : 1.0;
                mrg.outgoingEdges[0].emplace_back(weightedEdgeTransfer);
                        //.emplace( e.adjacentVertexId, mrg.node_id_assoc.getKey({m0, e}));
            }
        } else { // Otherwise, use the first edge
            const auto e = rg.outgoingEdges.at(m0);
            mrg.isInitialEStateAddedAfterwards = false;

            // Assertion zone
            assert(e.size() == 1);
            mrg.initialEState = mrg.node_id_assoc.getKey({m0, *e.begin()});
            for (const auto& u : rg.outgoingEdges) {
                for (const auto& e : u.second) {
                    assert(e.adjacentVertexId != m0);
                }
            }
        }

        // block end: freeing the memory from "phase1" and its temporary maps for e-closure
    }


    const auto fs = getFinalState(rg);
    if (fs.size() > 2/*doAddFState(rg, fs)*/) {
        mrg.finalEState = eSize_plusOne;
        mrg.isFinalEStateAddedAfterwards = true;
        HalfOfEdgeWithCost<Marking, size_t> weightedEdgeTransfer;
        weightedEdgeTransfer.cost = 1.0;
        for (const RGEdge<size_t,Marking> &out : fs) {
            weightedEdgeTransfer.halfEdge.edgeLabel        = out.edgeTarget.adjacentVertexId;
            weightedEdgeTransfer.halfEdge.adjacentVertexId = eSize_plusOne;
            mrg.outgoingEdges[mrg.node_id_assoc.getKey(out)].emplace_back(weightedEdgeTransfer);
               ///.emplace(out.edgeTarget.adjacentVertexId, eSize_plusOne);
        }
    } else if (fs.size() == 1) {
        auto it = fs.begin();
        mrg.finalEState = mrg.node_id_assoc.getKey(*it);
        mrg.isFinalEStateAddedAfterwards = false;
    }
    assert(fs.size() > 0);
    /*{
        std::ofstream of{"out/02_mrg.txt"};
        mrg.print(of, transition_labelling, eSize_plusOne);
    }*/

    /// Assertion zone
    std::cerr << "(performing assertion)" << std::flush;
    // 1) there should be only one vertex having no ingoing edges
    {
        std::map<size_t, std::unordered_set<HalfOfEdge<Marking, size_t>>> ingoingEdges2;
        for (const auto& cp: mrg.outgoingEdges) {
            for (const auto& e : cp.second) {

                ingoingEdges2[e.halfEdge.adjacentVertexId].emplace(e.halfEdge.edgeLabel, cp.first);
            }
        }
        size_t toCount = 0;
        for (const auto& v : mrg.node_id_assoc.getElements()) {
            if (ingoingEdges2.find(v.first) == ingoingEdges2.end()) {
                toCount++;
                std::cout << v.first << std::endl;
            }
        }
        if (mrg.isInitialEStateAddedAfterwards) {
            if (ingoingEdges2.find(mrg.initialEState) == ingoingEdges2.end()) {
                toCount++;
                std::cout << mrg.initialEState << std::endl;
            }
        }
        if (mrg.isFinalEStateAddedAfterwards) {
            if (ingoingEdges2.find(mrg.finalEState) == ingoingEdges2.end()) {
                toCount++;
                std::cout << mrg.finalEState << std::endl;
            }
        }
        assert(toCount == 1);
    }

    std::cerr << "done!" << std::endl;
    return mrg;
}

bool doAddIState(const ReachabilityGraph &rg, const Marking &m0) {
    return rg.outgoingEdges.find(m0)->second.size() > 1;
}

std::set<RGEdge<size_t,Marking>> getFinalState(const ReachabilityGraph &rg) {
    std::set<Marking > finalStates;
    for (const auto& state : rg.allStates) {
        const auto it = rg.outgoingEdges.find(state);
        if (it == rg.outgoingEdges.cend() || it->second.empty()) {
            finalStates.emplace(state);
            ///assert(finalStates.size() == 1);
        }
    }
    std::set<RGEdge<size_t,Marking>> toReturn;
    for (const auto& src : rg.outgoingEdges) {
        for (const auto& outE : src.second) {
            if (finalStates.find(outE.adjacentVertexId) != finalStates.end()) {
                toReturn.emplace( src.first, outE);
            }
        }
    }
    return toReturn;
}

bool doAddFState(const ReachabilityGraph &rg, const Marking &f0) {
    assert(false);
    /*size_t countForFinal = 0;
    for (const auto& cp : rg.outgoingEdges) {
        const auto it = cp.second.find(f0);
        if (it != cp.second.end()) countForFinal++;
    }
    assert(countForFinal > 0);
    return countForFinal != 1;*/
}


std::ostream &operator<<(std::ostream &out, const Transition_to_Marking &v) {
    out << '{';
    if ( !v.empty() ) {
        for (const auto& cp : v) {
            out << '\t' << cp.first << "-->" << cp.second << std::endl;
        }
    }
    out << "}";
    return out;
}

