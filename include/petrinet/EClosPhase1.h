//
// Created by giacomo on 28/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_ECLOSPHASE1_H
#define FUZZYSTRINGMATCHING2_ECLOSPHASE1_H


#include "PetriNet.h"
#include <Eigen/SparseCore>

using svector = Eigen::SparseVector<double> ;
bool operator==(const svector& __x, const svector& __y);
//std::ostream& operator<<(std::ostream& os, const svector& __x);
namespace std {
    template <>
    struct hash<svector> {
        size_t operator()(const svector& __x) {
            size_t init = 71;
            for (svector::InnerIterator it1(__x); it1; ++it1) {
                init = hash_combine(hash_combine(init, it1.index()), it1.value());
            }
            return init;
        }
    };
}

struct MemoryEfficientRGEdge2 {
    size_t          source_id;
    size_t          edgeLabel;
    size_t          target_id;
    double          probability;

    MemoryEfficientRGEdge2() : source_id{0}, edgeLabel{0}, target_id{0}, probability{1.0} {};
    MemoryEfficientRGEdge2(const MemoryEfficientRGEdge2&) = default;
    MemoryEfficientRGEdge2& operator=(const MemoryEfficientRGEdge2&) = default;
    MemoryEfficientRGEdge2(size_t sourceId, size_t edgeLabel, size_t targetId, double probability);

    bool operator<(const MemoryEfficientRGEdge2 &rhs) const;
    bool operator>(const MemoryEfficientRGEdge2 &rhs) const;
    bool operator<=(const MemoryEfficientRGEdge2 &rhs) const;
    bool operator>=(const MemoryEfficientRGEdge2 &rhs) const;
    bool operator==(const MemoryEfficientRGEdge2 &rhs) const;
    bool operator!=(const MemoryEfficientRGEdge2 &rhs) const;
};

namespace std {
    template <>
    struct hash<MemoryEfficientRGEdge2> {
        size_t operator()(const MemoryEfficientRGEdge2& __x) const {
            std::hash<double> doubleHash;
            return hash_combine(hash_combine(hash_combine(hash_combine(71, __x.source_id), __x.edgeLabel), __x.target_id), doubleHash( __x.probability));
        }
    };
}

/*struct MemoryEfficientRGEdge {
    svector                            source;
    size_t                             edgeLabel;
    svector                            target;
    double                             probability;

    MemoryEfficientRGEdge(const svector &source, size_t edgeLabel, const svector &target, double probability);
    MemoryEfficientRGEdge();
    MemoryEfficientRGEdge(const MemoryEfficientRGEdge&) = default;
    MemoryEfficientRGEdge(const RGEdge<size_t, Marking>& marking);
    MemoryEfficientRGEdge(const RGEdgeWithProbability<size_t, Marking>& marking);
    MemoryEfficientRGEdge(const RGEdge<size_t, Marking>& marking, double probability);
    MemoryEfficientRGEdge& operator=(const MemoryEfficientRGEdge&) = default;
    MemoryEfficientRGEdge& operator=(const RGEdge<size_t, Marking>& marking);
    MemoryEfficientRGEdge& operator=(const RGEdgeWithProbability<size_t, Marking>& marking);

    explicit operator RGEdge<size_t, Marking>()  const;

    bool operator==(const MemoryEfficientRGEdge &rhs) const;
    bool operator!=(const MemoryEfficientRGEdge &rhs) const;

private:
    void externalInit(const RGEdge<size_t, Marking>& marking);
    void externalInit(const RGEdgeWithProbability<size_t, Marking>& marking);
};

namespace std {
    template <>
    struct hash<MemoryEfficientRGEdge> {
        size_t operator()(const MemoryEfficientRGEdge& e) {
            std::hash<svector> ngri;
            return hash_combine(hash_combine(hash_combine(31, ngri(e.source)), e.edgeLabel), ngri(e.target));
        }
    };
}*/

class EClosPhase1 {
    PetriNet* lowLevel;
    ReachabilityGraph* rg;
    std::string epsilon;
    size_t markSize;
    fixed_bimap<svector, size_t> vectorCorrespondance;
    size_t vectorCorrespondanceSize = 0;

    svector fromStdMarking(const Marking& marking) const {
        svector result{static_cast<Eigen::Index>(marking.size())};
        for (size_t i = 0, N = marking.size(); i<N; i++)
            if (marking[i] != 0)
                result.coeffRef(i) = marking[i];
        return result;
    }

    Marking fromSVector(const svector& vec) const {
        Marking mark(markSize);
        for (svector::InnerIterator it1(vec); it1; ++it1) {
            mark[it1.index()] = it1.value();
        }
        return mark;
    }

    std::unordered_map<MemoryEfficientRGEdge2, std::unordered_set<MemoryEfficientRGEdge2>> map2;

    MemoryEfficientRGEdge2 toMemoryEfficient(const RGEdge<size_t, Marking>& key, double probability = 1.0) {
        MemoryEfficientRGEdge2 memo;
        memo.source_id = getVectorId(key.source);
        memo.edgeLabel = key.edgeTarget.edgeLabel;
        memo.target_id = getVectorId(key.edgeTarget.adjacentVertexId);
        memo.probability = probability;
        return memo;
    }

    std::unordered_set<MemoryEfficientRGEdge2>& mapGet(const RGEdge<size_t, Marking>& key, double probability = 1.0) {
        return map2[toMemoryEfficient(key, probability)];
    }

    void mapPut(const RGEdge<size_t, Marking>& key, const MemoryEfficientRGEdge2& val) {
        map2[toMemoryEfficient(key)].emplace(val);
    }

public:

    size_t getMapSizeVectorPreload() {
        ///assert(vectorCorrespondance.sanityCheck());
        return vectorCorrespondance.getSize();
    }

    RGEdgeWithProbability<size_t, Marking> expand(const MemoryEfficientRGEdge2& ref) {
        RGEdgeWithProbability<size_t, Marking> final;
        final.probability = ref.probability;
        final.edge.source = getVectorFromId(ref.source_id);
        final.edge.edgeTarget.edgeLabel = ref.edgeLabel;
        final.edge.edgeTarget.adjacentVertexId = getVectorFromId(ref.target_id);
        return final;
    }

    void mapPut(const RGEdge<size_t, Marking>& key, const RGEdge<size_t, Marking>& value_edge, double probability) {
        map2[toMemoryEfficient(key)].emplace(toMemoryEfficient(value_edge, probability));
    }

    void mapPutEmpty(const RGEdge<size_t, Marking>& key) {
        map2[toMemoryEfficient(key)] = {};
    }

    void setNewVectorId(const Marking& mark) {
        vectorCorrespondance.put(fromStdMarking(mark), vectorCorrespondanceSize++);
    }

    size_t getVectorId(const Marking& mark) {
        return vectorCorrespondance.getValue(fromStdMarking(mark));
    }

    size_t getOrSetVectorId(const Marking& mark) {
        const auto vec = fromStdMarking(mark);
        const auto it = vectorCorrespondance.containsKey2(vec);
        if (it) return it->second;
        else {
            vectorCorrespondance.put(vec, vectorCorrespondanceSize);
            return vectorCorrespondanceSize++;
        }
    }

    std::unordered_map<MemoryEfficientRGEdge2, std::unordered_set<MemoryEfficientRGEdge2>>::iterator mapFind(const RGEdge<size_t, Marking>& key, double probability = 1.0);
    std::unordered_map<MemoryEfficientRGEdge2, std::unordered_set<MemoryEfficientRGEdge2>>::iterator mapEnd();

    ///std::unordered_map<RGEdge<size_t, Marking>, std::unordered_set<RGEdgeWithProbability<size_t, Marking>>> map;
    EClosPhase1(PetriNet *lowLevel, ReachabilityGraph *rg, size_t markSize, const std::string& epsilon = ".");

    double getFiringCost(size_t transitionId) const;
    double getNormalizingFactor(const Marking& mark) const;
    bool isEpsilon(size_t transitionId) const;
    Marking getVectorFromId(size_t id);


    bool hasBeenVisited(const RGEdge<size_t, Marking>& edge);


    void eClos(const RGEdge<size_t, Marking>& e1);

};


#endif //FUZZYSTRINGMATCHING2_ECLOSPHASE1_H
