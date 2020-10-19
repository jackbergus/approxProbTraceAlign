//
// Created by giacomo on 15/10/20.
//

#include <numeric>
#include <iterator>
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

void
ReadGraph::generatePathEmbeddings(double lambda, ConditionalReadGraphIterable &iterable, path_to_uembedding &map,
                                  std::optional<double> optWeight, std::optional<size_t> optSource ) {
    iterable.resetParameters(optSource.value_or(source), target, &A, &inv_label_conversion);
    for (const auto& x : iterable) {
        ReadGraph::unstructured_embedding res = generatePathEmbedding(x.actualPath, lambda, optWeight.value_or(weight));
        map[x.path].emplace_back(res);
    }
}

void ReadGraph::decomposeStart(double lambda, ConditionalReadGraphIterable &iterable, path_to_uembedding &map) {
    for (Eigen::SparseMatrix<double, Eigen::RowMajor>::InnerIterator it(A,source); it; ++it){
        generatePathEmbeddings(lambda, iterable, map, {weight * it.value()}, {it.col()});
    }
}

ReadGraph::unstructured_embedding ReadGraph::generateWholeGraphEmbedding(double lambda) {
    Eigen::SparseMatrix<double, Eigen::RowMajor> current = A;
    ReadGraph::unstructured_embedding embedding;
    iterator it{embedding, lambda};
    while (current.nonZeros() > 0) {
        matrix_iterator(current, inv_label_conversion, it);
        it.nextIteration();
        current = current * A;
    }
    it.finalize(weight);
    return embedding;
}

void ReadGraph::print(const ReadGraph::unstructured_embedding &embedding) {
    for (const auto& it : embedding) {
        std::cout << '(' << it.first.first << ',' << it.first.second << ")/all = " << it.second << std::endl;
    }
}

ReadGraph::unstructured_embedding
ReadGraph::generatePathEmbedding(const std::vector<size_t> &path, double lambda, double weight) {
    ReadGraph rg;
    double pathCost = ReadGraph::insertPath(path, rg, inv_label_conversion, A);
    rg.finalizeEdgesMatrix(weight * pathCost);
    return rg.generateWholeGraphEmbedding(lambda);
}

double ReadGraph::insertPath(const std::vector<size_t> &path, ReadGraph &rg,
                             const std::unordered_map<size_t, std::string> &map,
                             Eigen::SparseMatrix<double, Eigen::RowMajor> &A) {
    size_t max = path.size();
    rg.init(*std::max_element(path.begin(), path.end())+1, max+1, path[0], path[max - 1]);
    double pathCost = 1;
    for (size_t i = 0; i<max-1; i++) {
        size_t j = path[i], k = path[i+1];
        rg.addNode(j, map.at(j));
        double cost = A.coeffRef(j, k);
        pathCost *= cost;
        rg.addEdge(j, path[i+1], cost);
    }
    rg.addNode(path[max-1], map.at(path[max - 1]));
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
    if (k.empty()) // Either using the exterior embedding, or calculate its own embedding space
        extractEmbeddingSpace(k, decomposedEmbedding);
    size_t j = 0;
    for (const auto& cpM: decomposedEmbedding) {
        for (const auto& x : cpM.second) {
            Eigen::VectorXd embedding(k.size());
            size_t i = 0;
            for (const auto& cp : k) {
                auto it = x.find(cp);
                embedding[i] = it == x.end() ? 0 : it->second;
                i++;
            }
            result.emplace(cpM.first, (embedding));
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
    }
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
