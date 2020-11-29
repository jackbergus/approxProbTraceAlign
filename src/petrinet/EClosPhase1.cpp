//
// Created by giacomo on 28/11/20.
//

#include "petrinet/EClosPhase1.h"

EClosPhase1::EClosPhase1(PetriNet *lowLevel, ReachabilityGraph *rg, size_t markSize, const std::string& epsilon)
        : lowLevel(lowLevel), rg(rg), epsilon(epsilon), markSize(markSize) {}

double EClosPhase1::getFiringCost(size_t transitionId) const {
    auto cost = lowLevel->firing_weight.find(transitionId);
    return cost !=  lowLevel->firing_weight.end() ? cost->second : 1.0;
}

double EClosPhase1::getNormalizingFactor(const Marking &mark) const {
    auto it = rg->outgoingEdges.find(mark);
    if (it == rg->outgoingEdges.end()) return 0.0;
    double toRet = 0.0;
    for (const auto& t : it->second) {
        toRet += getFiringCost(t.edgeLabel);
    }
    return toRet;
}

bool EClosPhase1::isEpsilon(size_t transitionId) const {
    const std::string retrieved = lowLevel->transition_labelling.at(transitionId);
    return retrieved == epsilon;
}

bool EClosPhase1::hasBeenVisited(const RGEdge<size_t, Marking> &edge) {
    return map2.find(toMemoryEfficient(edge))!= map2.end();
}

void EClosPhase1::eClos(const RGEdge<size_t, Marking> &e1) {
    if ((!isEpsilon(e1.edgeTarget.edgeLabel)) || (hasBeenVisited(e1))) return;

    ///std::cerr << " * " << e1 << " cachin' for ε!" << std::endl;
    double edgeNormalizationSum1 = getNormalizingFactor(e1.edgeTarget.adjacentVertexId);

    // Getting all the edges that are reachable after e1!
    auto it = rg->outgoingEdges.find(e1.edgeTarget.adjacentVertexId);
    if (it != rg->outgoingEdges.end()) {
        ///double testingSum = 0.0;
        RGEdge<size_t, Marking> e2;                                      // Constructing the reachable edge!
        e2.source = e1.edgeTarget.adjacentVertexId;                      // 1) Starting from the same vertex where e1 arrives
        for (const HalfOfEdge<size_t, Marking>& clos : it->second) {
            e2.edgeTarget = clos;                                        // 2) One of the other associated edges
                                                                         // -------------------------------------------

            double p = ((edgeNormalizationSum1 != 0.0) ? getFiringCost(clos.edgeLabel) / edgeNormalizationSum1 : 1.0);
            ///testingSum += p;

            // If this other edge is an epsilon one and not visited before, then I perform the caching for the closure
            bool isEps = isEpsilon(clos.edgeLabel);
            if (isEps && (!hasBeenVisited(e2))) {
                ///std::cerr << " * hitting recursion!" << std::endl;
                eClos(e2);
            }

            if (!isEps) {
                mapPut(e1, e2, p);
                // I add it directly to the map if and only if such edge is not e-labelled
                //map[e1].emplace(e2, p);
            } else {
                // If it is e-labelled, I only inherit its information.
                for (MemoryEfficientRGEdge2 costPairCopy : mapGet(e2)) {
                    costPairCopy.probability *= p;
                    mapPut(e1, costPairCopy);
                }
            }
        }

        ///assert(testingSum == 1.0); // Checking that the normalization associated to each of the outgoing edges is correct.
    } else {
        ///map[e1] = {}; // setting it as visited, but with an empty set of visited elements
        mapPutEmpty(e1);
    }
}

Marking EClosPhase1::getVectorFromId(size_t id) const {
    return fromSVector(vectorCorrespondance.getKey(id));
}

std::unordered_map<MemoryEfficientRGEdge2, std::unordered_set<MemoryEfficientRGEdge2>>::iterator
EClosPhase1::mapFind(const RGEdge<size_t, Marking> &key, double probability) {
    return map2.find(toMemoryEfficient(key, probability));
}

std::unordered_map<MemoryEfficientRGEdge2, std::unordered_set<MemoryEfficientRGEdge2>>::iterator EClosPhase1::mapEnd() {
    return map2.end();
}

/*
MemoryEfficientRGEdge::MemoryEfficientRGEdge(const svector &source, size_t edgeLabel, const svector &target,
                                             double probability) : source(source),
                                                                   edgeLabel(edgeLabel),
                                                                   target(target), probability(probability) {}

MemoryEfficientRGEdge::MemoryEfficientRGEdge() : source{0}, edgeLabel{0}, target{0}, probability{1.0} {}

MemoryEfficientRGEdge &MemoryEfficientRGEdge::operator=(const RGEdge<size_t, Marking> &marking) {
    externalInit(marking);
    return *this;
}

void MemoryEfficientRGEdge::externalInit(const RGEdge<size_t, Marking> &marking) {
    source.resize(marking.source.size());
    for (size_t i = 0, N = marking.source.size(); i<N; i++)
        if (marking.source[i] != 0)
            source.coeffRef(i) = marking.source[i];
    edgeLabel = marking.edgeTarget.edgeLabel;
    target.resize(marking.edgeTarget.adjacentVertexId.size());
    for (size_t i = 0, N = marking.edgeTarget.adjacentVertexId.size(); i<N; i++)
        if (marking.edgeTarget.adjacentVertexId[i] != 0)
            target.coeffRef(i) = marking.edgeTarget.adjacentVertexId[i];
    probability = 1.0;
}


MemoryEfficientRGEdge::MemoryEfficientRGEdge(const RGEdge<size_t, Marking> &marking) {
    externalInit(marking);
}

bool MemoryEfficientRGEdge::operator==(const MemoryEfficientRGEdge &rhs) const {
    return source == rhs.source &&
           edgeLabel == rhs.edgeLabel &&
           target == rhs.target;
}

bool MemoryEfficientRGEdge::operator!=(const MemoryEfficientRGEdge &rhs) const {
    return !(rhs == *this);
}

MemoryEfficientRGEdge::operator RGEdge<size_t, Marking>() const {
    RGEdge<size_t, Marking> summon;
    summon.source.resize(source.size(), 0);
    for (svector::InnerIterator it1(source); it1; ++it1) {
        summon.source[it1.index()] = it1.value();
    }
    summon.edgeTarget.adjacentVertexId.resize(target.size(), 0);
    for (svector::InnerIterator it1(target); it1; ++it1) {
        summon.edgeTarget.adjacentVertexId[it1.index()] = it1.value();
    }
    summon.edgeTarget.edgeLabel = edgeLabel;
    return summon;
}

void MemoryEfficientRGEdge::externalInit(const RGEdgeWithProbability<size_t, Marking> &marking) {
    externalInit(marking.edge);
    probability = marking.probability;
}

MemoryEfficientRGEdge::MemoryEfficientRGEdge(const RGEdgeWithProbability<size_t, Marking> &marking) {
    externalInit(marking);
}

MemoryEfficientRGEdge &MemoryEfficientRGEdge::operator=(const RGEdgeWithProbability<size_t, Marking> &marking) {
    externalInit(marking);
    return *this;
}

MemoryEfficientRGEdge::MemoryEfficientRGEdge(const RGEdge<size_t, Marking> &marking, double probability) : probability{probability} {
    externalInit(marking);
}*/

bool operator==(const svector &__x, const svector &__y) {
    for (svector::InnerIterator it1(__x), it2(__y); it1 && it2; )
    {
        if (it1.index() != it2.index()) {
            return false;
        } else if (it1.value() != it2.value()) {
            return false;
        }
        ++it1;
        ++it2;
    }
    return true;
}

/*std::ostream &operator<<(std::ostream &os, const svector &__x) {
    os << '{';
    for (svector::InnerIterator it1(__x); it1; ++it1) {
        os << '(' << it1.index() << ',' << it1.value() << ')';
    }
    return os << '}';
}*/

MemoryEfficientRGEdge2::MemoryEfficientRGEdge2(size_t sourceId, size_t edgeLabel, size_t targetId, double probability)
        : source_id(sourceId), edgeLabel(edgeLabel), target_id(targetId), probability(probability) {}

bool MemoryEfficientRGEdge2::operator<(const MemoryEfficientRGEdge2 &rhs) const {
    if (source_id < rhs.source_id)
        return true;
    if (rhs.source_id < source_id)
        return false;
    if (edgeLabel < rhs.edgeLabel)
        return true;
    if (rhs.edgeLabel < edgeLabel)
        return false;
    if (target_id < rhs.target_id)
        return true;
    if (rhs.target_id < target_id)
        return false;
    return probability < rhs.probability;
}

bool MemoryEfficientRGEdge2::operator>(const MemoryEfficientRGEdge2 &rhs) const {
    return rhs < *this;
}

bool MemoryEfficientRGEdge2::operator<=(const MemoryEfficientRGEdge2 &rhs) const {
    return !(rhs < *this);
}

bool MemoryEfficientRGEdge2::operator>=(const MemoryEfficientRGEdge2 &rhs) const {
    return !(*this < rhs);
}

bool MemoryEfficientRGEdge2::operator==(const MemoryEfficientRGEdge2 &rhs) const {
    return source_id == rhs.source_id &&
           edgeLabel == rhs.edgeLabel &&
           target_id == rhs.target_id &&
           probability == rhs.probability;
}

bool MemoryEfficientRGEdge2::operator!=(const MemoryEfficientRGEdge2 &rhs) const {
    return !(rhs == *this);
}
