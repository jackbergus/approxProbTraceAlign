//
// Created by giacomo on 11/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_GENERICGRAPH_H
#define FUZZYSTRINGMATCHING2_GENERICGRAPH_H

#include <unordered_map>
#include <vector>
#include <ostream>
#include <utils/pair_hash.h>
#include <cassert>
#include <unordered_set>
#include <utils/fixed_bimap.h>

template <typename id_type>
class GenericGraph {
    std::unordered_map<id_type, std::vector<std::pair<id_type, double>>> outgoingEdges;
    std::unordered_map<id_type, std::vector<std::pair<id_type, double>>> ingoingEdges;
    std::unordered_map<id_type, std::pair<std::string, double>>          node_labelled_weighted;
    id_type start;
    id_type end;
    double weight;
    std::string name;


    bool epsilonClosure(const std::string& epsilon = ".") {
        std::vector<id_type> epsilons;
        bool  someChange = false;
        size_t neu_start = start, neu_end = end;
        if ((node_labelled_weighted.at(start).first == epsilon) && (outgoing(start).size() == 1)) {
            auto cp = outgoingEdges.at(start)[0];
            neu_start = cp.first;
            remove_edge(start, neu_start);
            node_labelled_weighted.erase(start);
            weight *= cp.second;
            someChange = true;
        }
        if ((node_labelled_weighted.at(end).first == epsilon) && (ingoing(end).size() == 1)) {
            auto cp = ingoingEdges.at(end)[0];
            neu_end = cp.first;
            remove_edge(neu_end, end);
            node_labelled_weighted.erase(end);
            weight *= cp.second;
            someChange = true;
        }

        if (!someChange) {
            for (auto it = node_labelled_weighted.begin(), en = node_labelled_weighted.end(); it != en && (!someChange); it++) {
                if ((it->first != start) && (it->first != end) && (it->second.first == epsilon)) {
                    size_t eps = it->first;
                    std::vector<std::pair<id_type, double>> outs = outgoing(eps);
                    std::vector<std::pair<id_type, double>> ins = ingoing(eps);
                    for (const auto& e : outs) remove_edge(eps, e.first);
                    for (const auto& e : ins)  remove_edge(e.first, eps);
                    node_labelled_weighted.erase(eps);
                    someChange = (!outs.empty()) && (!ins.empty());
                    for (const auto& tgt: outs) {
                        for (const auto& src: ins) {
                            add_edge(src.first, tgt.first, src.second * tgt.second);
                        }
                    }
                }
            }
        }
        start = neu_start;
        end = neu_end;
        return someChange;
    }


public:
    GenericGraph() {}
    GenericGraph(const GenericGraph&) = default;
    GenericGraph& operator=(const GenericGraph&) = default;

    std::vector<size_t> getNodes() const {
        std::vector<size_t> result;
        result.reserve(node_labelled_weighted.size());
        for (const auto& cp: node_labelled_weighted) result.emplace_back(cp.first);
        return result;
    }

    size_t nodes() const {
        return node_labelled_weighted.size();
    }

    size_t countEdges() const {
        size_t count = 0;
        for (const auto& cp : outgoingEdges) {
            count += cp.second.size();
        }
        return count;
    }

    void setWeight(double weight) {
        GenericGraph::weight = weight;
    }

    void setStart(const id_type& start) {
        GenericGraph::start = start;
    }

    void setEnd(const id_type& end) {
        GenericGraph::end = end;
    }

    id_type getStart() const { return start; }
    size_t getEnd() const { return end; }
    const std::string &getName() const { return name; }
    double getCost() const { return weight; }

    void setName(const std::string &name) {
        GenericGraph::name = name;
    }

    std::string getNodeLabel(const id_type& id) const {
        return node_labelled_weighted.at(id).first;
    }

    double getNodeWeight(const id_type& id) const {
        return node_labelled_weighted.at(id).second;
    }

    std::vector<std::pair<id_type, double>> outgoing(const id_type& id) const {
        std::vector<std::pair<id_type, double>> v;
        auto it = outgoingEdges.find(id);
        return  (it != outgoingEdges.end()) ? it->second : v;
    }

    std::vector<std::pair<id_type, double>> ingoing(const id_type& id) const {
        std::vector<std::pair<id_type, double>> v;
        auto it = ingoingEdges.find(id);
        return  (it != ingoingEdges.end()) ? it->second : v;
    }

    void add_node(const id_type& id, const std::string& label, const double weight = 1.0) {
        node_labelled_weighted[id] = {label, weight};
    }

    void add_edge(const id_type& src, const id_type& dst, const double weight = 1.0) {
        outgoingEdges[src].emplace_back(dst, weight);
        ingoingEdges[dst].emplace_back(src, weight);
    }

    void update_edge_weight(std::unordered_map<std::pair<id_type, id_type>, double, pair_hash>& map) {
        std::pair<id_type,id_type> key{};
        for (auto& cp: outgoingEdges) {
            key.first = cp.first;
            for (auto& out : cp.second) {
                key.second = out.first;
                out.second = map.at(key);
            }
        }
        for (auto& cp: ingoingEdges) {
            key.second = cp.first;
            for (auto& out : cp.second) {
                key.first = out.first;
                out.second = map.at(key);
            }
        }
    }

    void transfer_weight_from_nodes_to_edges() {
        std::unordered_map<std::pair<id_type, id_type>, double, pair_hash> map;
        for (const auto& cp : outgoingEdges) {
            double summation = 0.0;
            for (const auto& out: cp.second) {
                summation+= node_labelled_weighted.at(out.first).second;
            }
            for (const auto& out: cp.second) {
                map.emplace(std::make_pair(cp.first, out.first), node_labelled_weighted.at(out.first).second / summation);
            }
        }
        update_edge_weight(map);
    }

    void remove_edge(const id_type& src, const id_type& dst) {
        {
            std::vector<std::pair<id_type, double>>& ingoing = ingoingEdges.at(dst);
            ingoing.erase(
                    std::remove_if(ingoing.begin(), ingoing.end(),
                                   [&src](const std::pair<id_type, double> & o) { return o.first == src; }),
                    ingoing.end());
        }

        {
            std::vector<std::pair<id_type, double>>& outgoing = outgoingEdges.at(src);
            outgoing.erase(
                    std::remove_if(outgoing.begin(), outgoing.end(),
                                   [&dst](const std::pair<id_type, double> & o) { return o.first == dst; }),
                    outgoing.end());
        }
    }

    void doClosure(const std::string& epsilon = ".") {
        while (epsilonClosure(epsilon));
    }


    friend std::ostream &operator<<(std::ostream &os, const GenericGraph<id_type> &graph) {
        os << "weight = " << graph.weight << " start = " << graph.start << " end = " << graph.end << std::endl;
        for (const auto& cp : graph.node_labelled_weighted) {
            os << "id = " << cp.first << " label = " << cp.second.first  << " weight = " << cp.second.second<< std::endl;
            auto it = graph.outgoingEdges.find(cp.first );
            if (it != graph.outgoingEdges.end()) {
                for (const auto& e : it->second) {
                    os << "\t\t * --[" << e.second << "]-->" << e.first << std::endl;
                }
            }
        }
        return os;
    }

    void inferTerminalNode() {
        bool found = false;
        for (const auto& cp : node_labelled_weighted) {
            auto it = outgoingEdges.find(cp.first);
            if ((it == outgoingEdges.end()) || (it->second.empty())) {
                assert(!found);
                end = cp.first;
                found = true;
            }
        }
    }

    void generateBimapLabels(fixed_bimap<std::string, char>& bimap, const std::string& admissible_chars, const std::string& eps) {
        std::unordered_set<std::string> visitedLabels;
        size_t nlabels = 0;
        for (const auto& cp : node_labelled_weighted) {
            if (cp.second.first == eps) continue;
            if (visitedLabels.insert(cp.second.first).second) {
                assert(nlabels < admissible_chars.size());
                bimap.put(cp.second.first, admissible_chars[nlabels++]);
            }
        }
    }

};


#endif //FUZZYSTRINGMATCHING2_GENERICGRAPH_H
