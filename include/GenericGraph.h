/*
 * GenericGraph.h
 * This file is part of ProbabilisticTraceAlignment
 *
 * Copyright (C) 2020 - Giacomo Bergami
 *
 * ProbabilisticTraceAlignment is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * ProbabilisticTraceAlignment is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ProbabilisticTraceAlignment. If not, see <http://www.gnu.org/licenses/>.
 */


//
// Created by giacomo on 11/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_GENERICGRAPH_H
#define FUZZYSTRINGMATCHING2_GENERICGRAPH_H

#include <unordered_map>
#include <map>
#include <vector>
#include <ostream>
#include <utils/pair_hash.h>
#include <cassert>
#include <unordered_set>
#include <utils/fixed_bimap.h>
#include <src/gvpp.hpp>
#include <graphviz/gvc.h>
#include <graphviz/cgraph.h>
#include <sstream>
#include <fstream>
#include <boost/unordered_map.hpp>

/*extern gvplugin_library_t gvplugin_dot_layout_LTX_library;
extern gvplugin_library_t gvplugin_neato_layout_LTX_library;
extern gvplugin_library_t gvplugin_core_LTX_library;
extern gvplugin_library_t gvplugin_quartz_LTX_library;*/

template <typename id_type>
class GenericGraph {
    boost::unordered_map<id_type, std::vector<std::pair<id_type, double>>> outgoingEdges;
    boost::unordered_map<id_type, std::vector<std::pair<id_type, double>>> ingoingEdges;
    boost::unordered_map<id_type, std::pair<std::string, double>>          node_labelled_weighted;
    id_type start;
    id_type end;
    double weight;
    std::string name;


    bool epsilonClosure(const std::string& epsilon = ".") {
        std::vector<id_type> epsilons;
        bool  someChange = false;

        ///if (!someChange) {

            for (auto it = node_labelled_weighted.cbegin(), en = node_labelled_weighted.cend(); it != en;) {
                if ((it->first != start) &&
                    (it->first != end) &&
                    (it->second.first == epsilon)) {
                    size_t eps = it->first;
                    std::cout << eps << "... Ok!" << std::endl;
                    std::vector<std::pair<id_type, double>> outs = outgoing(eps);
                    std::vector<std::pair<id_type, double>> ins = ingoing(eps);
                    for (const auto& e : outs) remove_edge(eps, e.first);
                    for (const auto& e : ins)  remove_edge(e.first, eps);
                    it = node_labelled_weighted.erase(it);
                    if (!someChange) someChange = someChange || ((!outs.empty()) && (!ins.empty()));
                    for (const auto& tgt: outs) {
                        for (const auto& src: ins) {
                            add_edge(src.first, tgt.first, src.second * tgt.second);
                        }
                    }
                    //if (someChange) break;
                } else {
                    it++;
                    //std::cout << std::endl;
                }
            }
        ///}

        if (someChange) return someChange;

        size_t neu_start = start, neu_end = end;
        if ((node_labelled_weighted.at(start).first == epsilon) && (outgoing(start).size() == 1)) {
            auto curr = outgoingEdges.at(start)[0];
            if (ingoing(start).empty()) {
                neu_start = curr.first;
                remove_edge(start, neu_start);
                node_labelled_weighted.erase(start);
                weight *= curr.second;
                someChange = true;
            } else {
                /*auto veccp = ingoing(start);
                for (const auto& cp : veccp) {
                    remove_edge(cp.first, start);
                    add_edge(cp.first, curr.first, cp.second * curr.second);
                }
                someChange = true;*/
            }
        }
        if ((node_labelled_weighted.at(end).first == epsilon) && (ingoing(end).size() == 1)) {
            assert((outgoing(end).empty()));
            auto cp = ingoingEdges.at(end)[0];
            neu_end = cp.first;
            remove_edge(neu_end, end);
            node_labelled_weighted.erase(end);
            weight *= cp.second;
            someChange = true;
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

    void updateNodeWeight(const id_type& id, double newCost) {
        node_labelled_weighted.at(id).second = newCost;
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
        std::vector<std::pair<id_type, id_type>> toRemove;
        for (const auto& cp : outgoingEdges) {
            double summation = 0.0;
            for (const auto& out: cp.second) {
                summation+= node_labelled_weighted.at(out.first).second;
            }

            for (const auto& out: cp.second) {
                double weight;
                if ((summation <= std::numeric_limits<double>::epsilon()) && (cp.second.size() == 1)) {
                    weight = 1.0;
                } else if (summation <= std::numeric_limits<double>::epsilon()) {
                    weight = 0.0;
                    toRemove.emplace_back(std::make_pair(cp.first, out.first));
                }  else {
                    weight = node_labelled_weighted.at(out.first).second / summation;
                }
                map.emplace(std::make_pair(cp.first, out.first), weight);
            }
        }
        for (const auto& cp : toRemove) {
            remove_edge(cp.first, cp.second);
        }
        if (!toRemove.empty())
            removeSolitaryNodes();
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
        size_t i = 0;
        while (epsilonClosure(epsilon)) { std::cerr << "Step " << i++ << std::endl; }
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

    void removeSolitaryNodes() {
        std::vector<id_type> toRemove;
        for (const auto& cp : node_labelled_weighted) {
            auto it = outgoingEdges.find(cp.first);
            auto it2 = ingoingEdges.find(cp.first);
            if (((it == outgoingEdges.end()) || (it->second.empty())) && ((it2 == ingoingEdges.end()) || (it2->second.empty()))) {
                if (it != outgoingEdges.end()) outgoingEdges.erase(it);
                if (it2 != ingoingEdges.end()) ingoingEdges.erase(it);
                toRemove.emplace_back(cp.first);
            }
        }
        for (const auto& u : toRemove) {
            const auto it = node_labelled_weighted.find(u);
            std::cerr << "Removing " << it->first << " with label " << it->second.first << " and weight " << it->second.second << std::endl;
            node_labelled_weighted.erase(it);
        }
    }

    void inferTerminalNode() {
        bool found = false;
        for (const auto& cp : node_labelled_weighted) {
            auto it = outgoingEdges.find(cp.first);
            if ((it == outgoingEdges.end()) || (it->second.empty())) {
                assert(!found); // ERROR: there are multiple terminal nodes (something fishy:  a likely bad input representation)
                end = cp.first;
                found = true;
            }
        }
        assert(found);// ERROR: no terminal node can be inferred (something fishy: it should exsist a node having no outgoing edges)
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


    void print(const std::string& filename) {
        std::ofstream file{filename};
        std::unordered_map<id_type, std::string> nodes;
        file << "start: " << start << " end:" << end << std::endl;
        for (const auto& n : node_labelled_weighted) {
            std::string label = n.second.first + " [w=" + std::to_string(n.second.second) +", id=" + std::to_string(n.first) +"]";
            std::string id = std::to_string(n.first);
            file << "vertex id: " << id << " label: " << label <<std::endl;
            nodes.emplace(n.first, id);
        }
        for (const auto& n : node_labelled_weighted) {
            for (const auto& out : outgoing(n.first)) {
                ///assert(node_labelled_weighted.find(out.first) != node_labelled_weighted.end());
                file << "edge " << n.first << "[" << n.second.first << "," << n.second.second << "]" << "-->" << out.first << std::endl;
            }
        }
    }


    bool render(const std::string& filename = "rendertest.pdf") {
        std::string str;
        {
            std::ostringstream oss;
            {
                gvpp::Graph<> render{};
                std::unordered_map<id_type, std::string> nodes;
                for (const auto& n : node_labelled_weighted) {
                    std::string label = n.second.first + " [w=" + std::to_string(n.second.second) +", id=" + std::to_string(n.first) +"]";
                    std::string id = std::to_string(n.first);
                    render.addNode(id, label);
                    nodes.emplace(n.first, id);
                }
                for (const auto& n : node_labelled_weighted) {
                    for (const auto& out : outgoing(n.first)) {
                        assert(node_labelled_weighted.find(out.first) != node_labelled_weighted.end());
                        //std::cerr << n.first << "[" << n.second.first << "," << n.second.second << "]" << "-->" << out.first << std::endl;
                        render.addEdge(render.getNode(nodes.at(n.first)), render.getNode(nodes.at(out.first)), std::to_string(out.second));
                    }

                }
                oss << render;
                ///renderToFile(render, "dot", "x11");
            }
            str = oss.str();
        }

        GVC_t *gvc;
        Agraph_t *gr;
        gvc = gvContext();
        //gvAddLibrary(gvc, &gvplugin_dot_layout_LTX_library);
        //gvAddLibrary(gvc, &gvplugin_neato_layout_LTX_library);
        //
        //gvAddLibrary(gvc, &gvplugin_core_LTX_library);
        //gvAddLibrary(gvc, &gvplugin_quartz_LTX_library);
        gr = agmemread(str.c_str());
        gvLayout(gvc, gr, "dot");
        gvRender(gvc, gr, "pdf", fopen(filename.c_str(), "w"));
        gvFreeLayout(gvc, gr);
        agclose(gr);
        return (gvFreeContext(gvc));
    }

};


#endif //FUZZYSTRINGMATCHING2_GENERICGRAPH_H
