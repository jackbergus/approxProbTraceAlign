//
// Created by giacomo on 24/11/20.
//

#include "PetriNet.h"

size_t PetriNet::w_cost(size_t p, size_t t)  {
    auto it = omega.find(p);
    if (it == omega.end()) return 0;
    auto it2 = it->second.find(t);
    return (it2 == it->second.end()) ? 0 : it2->second;
}

void PetriNet::w_set_cost(size_t p, size_t t, size_t cost) {
    omega[p][t] = cost;
}

bool PetriNet::is_transition_enabled(size_t transition_id, const PetriNet::Marking &marking)  {
    assert((places.size()) == marking.size());
    for (const size_t& place_id : places) {
        if (marking[(place_id)] < w_cost(place_id, transition_id)) return false;
    }
    return true;
}

std::unordered_set<size_t> PetriNet::enabling_transitions(const PetriNet::Marking &marking)  {
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

PetriNet::Transition_to_Marking PetriNet::generateOutgoingForReachabilityGraph(const PetriNet::Marking &marking)  {
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

PetriNet::ReachabilityGraph PetriNet::generateReachabilityGraph(const PetriNet::Marking &m0) {
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

PetriNet::MetaReachabilityGraph PetriNet::generateMetaReachabilityGraph(const PetriNet::Marking &m0) {
    std::cerr << "Generating the reachability graph... " << std::flush;
    auto rg = generateReachabilityGraph(m0);

    /*{
        std::ofstream of{"out/01_rg.txt"};
        rg.print(of, transition_labelling);
    }*/

    std::cerr << "done!" << std::endl << "Generating the Transition Graph from it... " << std::flush;

    PetriNet::MetaReachabilityGraph mrg;
    // Defining the vertices and their labelling from the graph's nodes
    size_t eSize_plusOne = 1;
    for (const auto& cp : rg.outgoingEdges) {
        for (const auto& e : cp.second) {
            mrg.node_id_assoc.put(eSize_plusOne++, std::make_pair(cp.first, e));
            eSize_plusOne++;
        }
    }
    // Defining the edges by the two-step edges
    for (const auto& v_out : rg.outgoingEdges) {
        for (const auto& e : v_out.second) {
            const auto it = rg.outgoingEdges.find(e.adjacentVertexId);
            if (it != rg.outgoingEdges.end()) {
                for (const auto& e2 : it->second) {
                    mrg.outgoingEdges[mrg.node_id_assoc.getKey(std::make_pair(v_out.first, e))]
                        .emplace( e.adjacentVertexId, mrg.node_id_assoc.getKey(std::make_pair(e.adjacentVertexId, e2)));
                }
            }
        }
    }
    if (doAddIState(rg, m0)) { // If I need to add an explicit initial state, because there are many edges starting from the initial node
        mrg.initialEState = 0;
        mrg.isInitialEStateAddedAfterwards = true;
        for (const auto& e : rg.outgoingEdges.at(m0)) {
            mrg.outgoingEdges[0]
               .emplace( e.adjacentVertexId, mrg.node_id_assoc.getKey(std::make_pair(m0, e)));
        }
    } else { // Otherwise, use the first edge
        const auto e = rg.outgoingEdges.at(m0);
        mrg.isInitialEStateAddedAfterwards = false;
        assert(e.size() == 1);
        mrg.initialEState = mrg.node_id_assoc.getKey(std::make_pair(m0, *e.begin()));
        for (const auto& u : rg.outgoingEdges) {
            for (const auto& e : u.second) {
                assert(e.adjacentVertexId != m0);
            }
        }
    }

    const auto fs = getFinalState(rg);
    if (fs.size() > 2/*doAddFState(rg, fs)*/) {
        mrg.finalEState = eSize_plusOne;
        mrg.isFinalEStateAddedAfterwards = true;
        for (const auto &out : fs) {
            /*const auto it = out.second.find(fs);
            if (it != out.second.end()) {
            }*/
            mrg.outgoingEdges[mrg.node_id_assoc.getKey(out)]
               .emplace(out.second.adjacentVertexId, eSize_plusOne);
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
                ingoingEdges2[e.adjacentVertexId].emplace(e.edgeLabel, cp.first);
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

bool doAddIState(const PetriNet::ReachabilityGraph &rg, const PetriNet::Marking &m0) {
    return rg.outgoingEdges.find(m0)->second.size() > 1;
}

std::set<PetriNet::ReachabilityGraphEdge> getFinalState(const PetriNet::ReachabilityGraph &rg) {
    std::set<PetriNet::Marking > finalStates;
    for (const auto& state : rg.allStates) {
        const auto it = rg.outgoingEdges.find(state);
        if (it == rg.outgoingEdges.cend() || it->second.empty()) {
            finalStates.emplace(state);
            ///assert(finalStates.size() == 1);
        }
    }
    std::set<PetriNet::ReachabilityGraphEdge> toReturn;
    for (const auto& src : rg.outgoingEdges) {
        for (const auto& outE : src.second) {
            if (finalStates.find(outE.adjacentVertexId) != finalStates.end()) {
                toReturn.insert( std::make_pair(src.first, outE));
            }
        }
    }
    return toReturn;
}

bool doAddFState(const PetriNet::ReachabilityGraph &rg, const PetriNet::Marking &f0) {
    assert(false);
    /*size_t countForFinal = 0;
    for (const auto& cp : rg.outgoingEdges) {
        const auto it = cp.second.find(f0);
        if (it != cp.second.end()) countForFinal++;
    }
    assert(countForFinal > 0);
    return countForFinal != 1;*/
}

#include <iterator> // needed for std::ostram_iterator
std::ostream &operator<<(std::ostream &out, const  std::vector<size_t> &v) {
    out << '(';
    if ( !v.empty() ) {
        std::copy (v.begin(), v.end(), std::ostream_iterator<size_t>(out, ","));
    }
    out << ")";
    return out;
}

std::ostream &operator<<(std::ostream &out, const PetriNet::ReachabilityGraphEdge &e) {
    return out << e.first << "-[" << e.second.edgeLabel << "]->" << e.second.adjacentVertexId;
}

std::ostream &operator<<(std::ostream &out, const PetriNet::Transition_to_Marking &v) {
    out << '{';
    if ( !v.empty() ) {
        for (const auto& cp : v) {
            out << '\t' << cp.first << "-->" << cp.second << std::endl;
        }
    }
    out << "}";
    return out;
}

std::ostream &PetriNet::ReachabilityGraph::print(std::ostream &os, const std::unordered_map<size_t, std::string> &tl) {
    for (const auto& it : outgoingEdges) {
        for (const auto &it2 : it.second) {
            os << it.first << "-[" << tl.at(it2.edgeLabel) << "]->" << it2.adjacentVertexId << std::endl;
        }
    }
    return os;
}
