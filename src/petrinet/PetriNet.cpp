//
// Created by giacomo on 24/11/20.
//

#include "petrinet/PetriNet.h"
#include "utils/ProgressBar.h"

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
        EClosPhase1 phase1{this, &rg, m0.size()};

        /*{
            std::ofstream of{"out/01_rg.txt"};
            rg.print(of, transition_labelling);
        }*/


        {

            progress_bar bar{std::cout, 70u, "Preloading the vectors"};
            double vecId = 0.0, vecSize = rg.allStates.size();
            for (const Marking& marking : rg.allStates) {
                phase1.setNewVectorId(marking);
                bar.write((++vecId)/vecSize);
            }
            assert(rg.allStates.size() == phase1.getMapSizeVectorPreload());
        }


        // Defining the vertices and their labelling from the graph's nodes
        RGEdge<size_t, Marking> edge_to_vector;
        {
            progress_bar bar{std::cout, 70u, "Generating non-e vertices with closure"};
            double count = 0, size = rg.outgoingEdges.size();
            for (const auto& vertex_to_outgoingEdges : rg.outgoingEdges) {
                edge_to_vector.source = vertex_to_outgoingEdges.first;              // | * Vertex
                for (const auto& outgoingEdge : vertex_to_outgoingEdges.second) {
                    edge_to_vector.edgeTarget        = outgoingEdge;                // | * Vertex's outgoing edges
                    // edge_to_vector is now fully characterized as an edge from rg // |--------------------------|

                    std::string label                = transition_labelling.at(outgoingEdge.edgeLabel);

                    // If this edge is an epsilon-labelled one, I want to perform the closure, nevertheles
                    if (label == epsilon) {
                        phase1.eClos(edge_to_vector);
                    }

                    // Adding it as a node if and only if it is a starting node,
                    // or if it is a final node, or if it is a non-epsilon node
                    auto outgoingEdge_targetOutgoing = rg.outgoingEdges.find(outgoingEdge.adjacentVertexId);
                    if ((edge_to_vector.source == m0)                           || // --> starting node
                        (outgoingEdge_targetOutgoing == rg.outgoingEdges.end()) || // --> 1) final node
                        (outgoingEdge_targetOutgoing->second.empty())           || // --> 2) final node
                        (label != epsilon)                                         // --> non-epsilon
                            )
                    {
                        mrg.node_id_assoc.put(eSize_plusOne++, edge_to_vector);    // Storing the edge as a vector!
                    }
                }
                bar.write(((++count)/size));
            }
        }

        ///std::cerr << "done!" << std::endl << "Generating the Transition Graph from it (2: edge scan, +closure part 2)... " << std::flush;

        // Defining the edges by the two-step edges
        {
            progress_bar bar{std::cout, 70u, "Generating edges among the non-e vertices"};
            double count = 0, size = mrg.node_id_assoc.getElements().size();
            for (const auto& nonepsilon_id_with_metaVertex : mrg.node_id_assoc.getElements())  {
                const RGEdge<size_t, Marking> metaVertex1    = nonepsilon_id_with_metaVertex.second;
                size_t                        metaVertex1_id = nonepsilon_id_with_metaVertex.first;

                // The normalizatioin is epressed over the possible edges that I could reach. Therefore,
                // it is defined from the outgoing edges from the metavertex's target node
                double normalizingCost = phase1.getNormalizingFactor(metaVertex1.edgeTarget.adjacentVertexId);

                RGEdge<size_t, Marking>       metaVertex2;                                      // Second vertex to connect via edge:
                metaVertex2.source = metaVertex1.edgeTarget.adjacentVertexId;                   // 1) From target to source

                // Getting the actual edges from the Reachability Graph
                const auto it = rg.outgoingEdges.find(metaVertex1.edgeTarget.adjacentVertexId);
                if (it != rg.outgoingEdges.end()) {
                    for (const auto& rawMetaVertex2 : it->second) {
                        metaVertex2.edgeTarget = rawMetaVertex2;                                // 2) Finishing towards the new target
                        // -----------------------------------

                        auto isMetaVertex2It        = phase1.mapFind(metaVertex2);
                        bool isMetaVertex2NotClosed = isMetaVertex2It == phase1.mapEnd();

                        double p = (normalizingCost != 0.0)                                                     ?
                                   (phase1.getFiringCost(metaVertex2.edgeTarget.edgeLabel) / normalizingCost) :
                                   1.0;

                        if (isMetaVertex2NotClosed) {
                            // if tmp2 is not an e-edge, and therefore it is not in the e-map, I directly add such
                            // edge!
                            /**HalfOfEdgeWithProbability<Marking, size_t> weightedEdgeTransfer;
                            weightedEdgeTransfer.halfEdge.edgeLabel        = metaVertex2.source;
                            weightedEdgeTransfer.halfEdge.adjacentVertexId = mrg.node_id_assoc.getKey(metaVertex2);
                            weightedEdgeTransfer.probability                      = p;*/
                            mrg.outgoingEdges[metaVertex1_id].emplace(mrg.node_id_assoc.getKey(metaVertex2), p);
                            ///mrg.outgoingEdges[metaVertex1_id].emplace_back(weightedEdgeTransfer);
                        } else {
                            // I inherit all the outgoing edges that are associated to it via closure
                            for (const MemoryEfficientRGEdge2& local : isMetaVertex2It->second) {
                                RGEdgeWithProbability<size_t, Marking> weightedEdge  = phase1.expand(local);
                                /**HalfOfEdgeWithProbability<Marking, size_t> weightedEdgeTransfer;
                                weightedEdgeTransfer.halfEdge.edgeLabel        = weightedEdge.edge.source;
                                weightedEdgeTransfer.halfEdge.adjacentVertexId = mrg.node_id_assoc.getKey(weightedEdge.edge);
                                weightedEdgeTransfer.probability                      = p * weightedEdge.probability;*/
                                mrg.outgoingEdges[metaVertex1_id].emplace(mrg.node_id_assoc.getKey(weightedEdge.edge), p * weightedEdge.probability);
                                ///mrg.outgoingEdges[metaVertex1_id].emplace_back(weightedEdgeTransfer);
                            }
                        }

                    }
                }
                bar.write(((++count)/size));
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
                /*HalfOfEdgeWithProbability<Marking, size_t> weightedEdgeTransfer;
                weightedEdgeTransfer.halfEdge.edgeLabel        = m0;
                weightedEdgeTransfer.halfEdge.adjacentVertexId = mrg.node_id_assoc.getKey(m0out);
                weightedEdgeTransfer.probability                      = (normalizingCost != 0.0) ? phase1.getFiringCost(e.edgeLabel) / normalizingCost : 1.0;*/
                mrg.outgoingEdges[0].emplace(mrg.node_id_assoc.getKey(m0out), ((normalizingCost != 0.0) ? phase1.getFiringCost(e.edgeLabel) / normalizingCost : 1.0));
                ///mrg.outgoingEdges[0].emplace_back(weightedEdgeTransfer);
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
        /**HalfOfEdgeWithProbability<Marking, size_t> weightedEdgeTransfer;
        weightedEdgeTransfer.probability = 1.0;*/
        for (const RGEdge<size_t,Marking> &out : fs) {
            /**weightedEdgeTransfer.halfEdge.edgeLabel        = out.edgeTarget.adjacentVertexId;
            weightedEdgeTransfer.halfEdge.adjacentVertexId = eSize_plusOne;*/
            mrg.outgoingEdges[mrg.node_id_assoc.getKey(out)].emplace(eSize_plusOne, 1.0);
            ///mrg.outgoingEdges[mrg.node_id_assoc.getKey(out)].emplace_back(weightedEdgeTransfer);
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
        std::map<size_t, std::unordered_set<size_t>> ingoingEdges2;
        for (const auto& cp: mrg.outgoingEdges) {
            for (const auto& e : cp.second) {
                ingoingEdges2[e.dst].emplace(e.dst);
                ///ingoingEdges2[e.halfEdge.adjacentVertexId].emplace(e.halfEdge.edgeLabel, cp.first);
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
        if(toCount != 1) {
            std::cerr << "Warning: something seems not ok!" << std::endl;
        }
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

