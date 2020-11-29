//
// Created by giacomo on 28/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_RGEDGE_H
#define FUZZYSTRINGMATCHING2_RGEDGE_H

#include <petrinet/HalfOfEdge.h>

template <typename EdgeLabelType, typename VertexIdType>
struct RGEdge {
    VertexIdType                            source;
    HalfOfEdge<EdgeLabelType, VertexIdType> edgeTarget;

    RGEdge() = default;
    RGEdge(const RGEdge&) = default;
    RGEdge& operator=(const RGEdge&) = default;

    RGEdge(VertexIdType source, const HalfOfEdge<EdgeLabelType, VertexIdType> &edgeTarget) :
    source(source), edgeTarget(edgeTarget) {}

    friend std::ostream &operator<<(std::ostream &os, const RGEdge &edge) {
        return os << edge.source << "->[" << edge.edgeTarget << "]";
    }

    bool operator<(const RGEdge &rhs) const {
        if (source < rhs.source) return true;
        if (rhs.source < source) return false;
        return edgeTarget < rhs.edgeTarget;
    }

    bool operator==(const RGEdge &rhs) const { return source == rhs.source && edgeTarget == rhs.edgeTarget; }
    bool operator!=(const RGEdge &rhs) const { return !(rhs == *this); }
    bool operator>(const RGEdge &rhs) const { return rhs < *this; }
    bool operator<=(const RGEdge &rhs) const { return !(rhs < *this); }
    bool operator>=(const RGEdge &rhs) const { return !(*this < rhs); }
};

namespace std {
    template <typename EdgeLabelType, typename VertexIdType>
    struct hash<RGEdge<EdgeLabelType,VertexIdType>> {
        std::size_t operator()(const RGEdge<EdgeLabelType,VertexIdType>& k) const {
            std::hash<HalfOfEdge<EdgeLabelType, VertexIdType>> edgeHasher;
            std::hash<VertexIdType> vertexHasher;
            return hash_combine(hash_combine(71, edgeHasher(k.edgeTarget)), vertexHasher(k.source));
        }
    };
}

template <typename EdgeLabelType, typename VertexIdType>
struct RGEdgeWithProbability {
    RGEdge<EdgeLabelType, VertexIdType> edge;
    double probability;

    RGEdgeWithProbability() {}
    RGEdgeWithProbability(const RGEdge<EdgeLabelType, VertexIdType> &edge, double cost) : edge(edge), probability(cost) {}
    RGEdgeWithProbability(const RGEdgeWithProbability&) = default;
    RGEdgeWithProbability& operator=(const RGEdgeWithProbability&) = default;

    bool operator<(const RGEdgeWithProbability &rhs) const {
        if (edge < rhs.edge) return true;
        if (rhs.edge < edge) return false;
        return probability < rhs.probability;
    }
    bool operator>(const RGEdgeWithProbability &rhs) const  { return rhs < *this;    }
    bool operator<=(const RGEdgeWithProbability &rhs) const { return !(rhs < *this); }
    bool operator>=(const RGEdgeWithProbability &rhs) const { return !(*this < rhs); }
    bool operator==(const RGEdgeWithProbability &rhs) const { return edge == rhs.edge && probability == rhs.probability; }
    bool operator!=(const RGEdgeWithProbability &rhs) const { return !(rhs == *this); }
};

namespace std {
    template <typename EdgeLabelType, typename VertexIdType>
    struct hash<RGEdgeWithProbability<EdgeLabelType,VertexIdType>> {
        std::size_t operator()(const RGEdgeWithProbability<EdgeLabelType,VertexIdType>& k) const {
            std::hash<RGEdge<EdgeLabelType, VertexIdType>> edgeHasher;
            std::hash<double> costHasher;
            return hash_combine(hash_combine(71, edgeHasher(k.edge)), costHasher(k.probability));
        }
    };
}

#endif //FUZZYSTRINGMATCHING2_RGEDGE_H
