//
// Created by giacomo on 15/10/20.
//

#include <numeric>
#include <iterator>
#include <ThomsonNFA.h>
#include "ReadGraph.h"



ReadGraph::ReadGraph(const std::string &filename) {
    FILE* file = fopen(filename.c_str(), "r");
    if (file) {
        int i;
        double w = 1;
        // Reading the number of the nodes
        i = fscanf(file, "nodes: %zd\n", &nodes);
        error = (i == EOF || (i != 1));
        if (error) return;
        i = fscanf(file, "edges: %zd\n", &edges);
        error = (i == EOF || (i != 1));
        if (error) return;
        i = fscanf(file, "source: %zd\n", &source);
        error = (i == EOF || (i != 1));
        if (error) return;
        i = fscanf(file, "target: %zd\n", &target);
        error = (i == EOF || (i != 1));
        if (error) return;
        i = fscanf(file, "weight: %lf\n", &w);
        error = (i == EOF || (i != 1));
        if (error) return;

        for (size_t j = 0; j<nodes; j++) {
            size_t node_no;
            char string[124];
            std::string k;
            i = fscanf(file, "%zd %123s\n", &node_no, string);
            error = (i == EOF || (i != 2));
            if (error) return;
            k = std::string(string);
            if (!addNode(node_no, k)) return;
        }


        for (size_t j = 0; j<edges; j++) {
            size_t src, dst;
            double weight;
            i = fscanf(file, "%zd %zd %lf\n", &src, &dst, &weight);
            error = (i == EOF || (i != 3));
            if (error) return;
            assert(std::abs(weight) <= 1.0);
            addEdge(src, dst, weight);
        }
        finalizeEdgesMatrix(w);
        fclose(file);
    }
}

void ReadGraph::init(size_t vSize, size_t eSize, size_t src, size_t tgt) {
    nodes = vSize;
    edges = eSize;
    source = src;
    target = tgt;
}

bool ReadGraph::addNode(size_t id, const std::string &label) {
    return inv_label_conversion.insert(std::make_pair(id, label)).second;
}

void ReadGraph::addEdge(size_t src, size_t dst, double weight) {
    assert(0 <= weight && weight <= 1);
    if (weight != 0) tripletList.emplace_back(src,dst,weight);
}

void ReadGraph::finalizeEdgesMatrix(double cost) {
    weight = cost;
    Eigen::SparseMatrix<double, Eigen::RowMajor> matrix(nodes, nodes);
    matrix.setFromTriplets(tripletList.begin(), tripletList.end());
    A = matrix;
}


void ReadGraph::decomposeStart(double lambda, ConditionalReadGraphIterable &iterable, path_to_uembedding &map) {
    assert(false && "TODO: you have to implement it!");
    /*for (Eigen::SparseMatrix<double, Eigen::RowMajor>::InnerIterator it(A,source); it; ++it){
        generatePathEmbeddings(lambda, iterable, map, {weight * it.value()}, {it.col()});
    }*/
}



void ReadGraph::print(const ReadGraph::unstructured_embedding &embedding) {
    for (const auto& it : embedding) {
        std::cout << '(' << it.first.first << ',' << it.first.second << ")/all = " << it.second << std::endl;
    }
}

double ReadGraph::generateGraphFromPath(const std::vector<size_t> &path, ReadGraph &rg,
                                        const std::unordered_map<size_t, std::string> &nodeLabelling,
                                        Eigen::SparseMatrix<double, Eigen::RowMajor> &edge_weight_matrix) {
    size_t max = path.size();
    size_t source = path[0];
    size_t target = path[max-1];
    double pathCost = 1;
    double graphCost = 1.0;
    for (size_t i = 0; i<max-1; i++) {
        size_t j = path[i], k = path[i+1];
        if ((nodeLabelling.at(j) == EPSILON))  {
            assert(i == 0);
            graphCost *= edge_weight_matrix.coeffRef(j, k);
            source = path[1];
            continue;
        }
        if ((nodeLabelling.at(k) == EPSILON)) {
            assert(i == max-2);
            graphCost *= edge_weight_matrix.coeffRef(j, k);
            target = path[max-2];
            continue;
        }
        rg.addNode(j, nodeLabelling.at(j));
        double cost = edge_weight_matrix.coeffRef(j, k);
        pathCost *= cost;
        rg.addEdge(j, k, cost);
    }
    rg.init(*std::max_element(path.begin(), path.end())+1, max+1, source, target);
    rg.addNode(target, nodeLabelling.at(target));
    rg.weight = graphCost;
    return pathCost;
}

void ReadGraph::removeNode(size_t k) {
    for (Eigen::SparseMatrix<double, Eigen::RowMajor>::InnerIterator it(A,k); it; ++it)
        it.valueRef() = 0.0;
    inv_label_conversion.erase(k);
}

#include <set>

std::unordered_map<std::string, Eigen::VectorXd>
ReadGraph::generateStructuredEmbeddings(std::set<std::pair<std::string, std::string>> &k, const path_to_uembedding &decomposedEmbedding) {
    size_t i = 0;
    std::unordered_map<std::string, Eigen::VectorXd> result;
    assert(!k.empty());
    size_t j = 0;
    for (const auto& cpM: decomposedEmbedding) {
        for (const auto& x : cpM.second) {
            result.emplace(cpM.first, ReadGraph::generateStructuredEmbedding(k, x));
        }
    }
    return result;
}

void ReadGraph::extractEmbeddingSpace(std::set<std::pair<std::string, std::string>> &k,
                                      const path_to_uembedding& decomposedEmbedding) {
    for (const auto& x : decomposedEmbedding) {
        for (const ReadGraph::unstructured_embedding& y : x.second) {
            for (const auto& e : y) {
                k.insert(e.first);
            }
        }
    }//ReadGraph::unstructured_embedding
}

void matrix_print(const Eigen::SparseMatrix<double, Eigen::RowMajor>& A, const std::unordered_map<size_t, std::string>& map) {
    for (int k=0; k < A.outerSize(); ++k)
    {
        for (Eigen::SparseMatrix<double, Eigen::RowMajor>::InnerIterator it(A,k); it; ++it)
        {
            size_t i = it.row(), j = it.col();
            double v = it.value();
            std::cout << i << '(' << map.at(i) << ") =[" << v << "]=>" << j << '(' << map.at(j) << ')' << std::endl;
        }
    }
}

void ReadGraph::printGraph() const {
    matrix_print(A, inv_label_conversion);
}

PathIterator ReadGraph::javaPathIterator(bool doNotVisitLoopsTwice, size_t maxPathLength, const double minimumPathCost) {
    return {&A,
            &inv_label_conversion,
            maxPathLength,
            minimumPathCost,
            doNotVisitLoopsTwice};
}

ConditionalReadGraphIterable
ReadGraph::iterateOverPaths(bool doNotVisitLoopsTwice, size_t maxPathLength, const double minimumPathCost) {
    return {this->source, this->target, this->weight, &A, &inv_label_conversion, maxPathLength, minimumPathCost, doNotVisitLoopsTwice};
}

void ReadGraph::extractEmbeddingSpace(std::set<std::pair<std::string, std::string>> &k,
                                      const ReadGraph::unstructured_embedding &y) {
    for (const auto& e : y) {
        k.insert(e.first);
    }
}
