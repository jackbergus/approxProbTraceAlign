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

#endif //FUZZYSTRINGMATCHING2_RGEDGE_H
