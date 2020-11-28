//
// Created by giacomo on 28/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_HALFOFEDGE_H
#define FUZZYSTRINGMATCHING2_HALFOFEDGE_H

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
            return hash_combine(hash_combine(31, edgeHasher(k.edgeLabel)), vertexHasher(k.adjacentVertexId));
        }
    };
}

template <typename EdgeLabelType, typename VertexIdType>
struct HalfOfEdgeWithCost {
    HalfOfEdge<EdgeLabelType, VertexIdType> halfEdge;
    double cost;

    HalfOfEdgeWithCost() {}
    HalfOfEdgeWithCost(const HalfOfEdge<EdgeLabelType, VertexIdType> &edge, double cost) : halfEdge(edge), cost(cost) {}
    HalfOfEdgeWithCost(const HalfOfEdgeWithCost&) = default;
    HalfOfEdgeWithCost& operator=(const HalfOfEdgeWithCost&) = default;

    bool operator<(const HalfOfEdgeWithCost &rhs) const {
        if (halfEdge < rhs.halfEdge) return true;
        if (rhs.halfEdge < halfEdge) return false;
        return cost < rhs.cost;
    }
    bool operator>(const HalfOfEdgeWithCost &rhs) const  { return rhs < *this;    }
    bool operator<=(const HalfOfEdgeWithCost &rhs) const { return !(rhs < *this); }
    bool operator>=(const HalfOfEdgeWithCost &rhs) const { return !(*this < rhs); }
    bool operator==(const HalfOfEdgeWithCost &rhs) const { return halfEdge == rhs.halfEdge && cost == rhs.cost; }
    bool operator!=(const HalfOfEdgeWithCost &rhs) const { return !(rhs == *this); }
};

namespace std {
    template <typename EdgeLabelType, typename VertexIdType>
    struct hash<HalfOfEdgeWithCost<EdgeLabelType,VertexIdType>> {
        std::size_t operator()(const HalfOfEdgeWithCost<EdgeLabelType,VertexIdType>& k) const {
            std::hash<HalfOfEdge<EdgeLabelType, VertexIdType>> edgeHasher;
            std::hash<double> costHasher;
            return hash_combine(hash_combine(71, edgeHasher(k.halfEdge)), costHasher(k.cost));
        }
    };
}

#endif //FUZZYSTRINGMATCHING2_HALFOFEDGE_H
