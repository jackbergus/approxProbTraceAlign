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

void ReadGraph::generateDecomposedEmbeddings(double lambda) {
    // Mark all the vertices as not visited
    std::vector<bool> visited;
    pathNameList.clear();
    visited.insert(visited.begin(), nodes, false);
    std::vector<size_t> path;
    ///path.reserve(nodes);
    size_t path_index = 0; // Initialize path[] as empty

    // Call the recursive helper function to print all paths
    generateAllPossibleSubgraphsFromPaths(source, target, visited, path, path_index, lambda);
}

void ReadGraph::decomposeStart(double lambda) {
    std::vector<std::pair<size_t, double>> e;
    double tmp = weight;
    pathNameList.clear();
    for (Eigen::SparseMatrix<double, Eigen::RowMajor>::InnerIterator it(A,source); it; ++it){
        ///std::cerr << "Decompose" << std::endl;
        std::vector<bool> visited;
        visited.insert(visited.begin(), nodes, false);
        std::vector<size_t> path;
        size_t path_index = 0;
        weight = tmp * it.value();
        size_t src = it.col();
        generateAllPossibleSubgraphsFromPaths(src, target, visited, path, path_index, lambda);
    }
}

void ReadGraph::generateEmbeddings(double lambda) {
    Eigen::SparseMatrix<double, Eigen::RowMajor> current = A;
    size_t iteration = 1;
    iterator it{embedding, lambda};
    while (current.nonZeros() > 0) {
        matrix_iterator(current, inv_label_conversion, it);
        it.nextIteration();
        current = current * A;
    }
    it.finalize(weight);
}

void ReadGraph::printLocalEmbedding() {
    print(embedding);
}

void ReadGraph::printAllPathsEmbeddings() {
    size_t i = 0;
    for (const auto& x : decomposedEmbedding) {
        std::cout << i << ":" << std::endl;
        print(x);
        std::cout << std::endl << std::endl;
        i++;
    }
}

void ReadGraph::print(const std::unordered_map<std::pair<std::string, std::string>, double, pair_hash> &embedding) {
    for (const auto& it : embedding) {
        std::cout << '(' << it.first.first << ',' << it.first.second << ")/all = " << it.second << std::endl;
    }
}

void ReadGraph::generatePathEmbedding(std::vector<size_t> &path, size_t max, double lambda) {
    ///std::cerr << "NewPath" << std::endl;
    ReadGraph rg;
    double pathCost = insertPath(path, max, rg);
    rg.finalizeEdgesMatrix(weight * pathCost);
    rg.generateEmbeddings(lambda);
    decomposedEmbedding.emplace_back(rg.embedding);
}

double ReadGraph::insertPath(const std::vector<size_t> &path, size_t max, ReadGraph &rg) {
    rg.init(nodes, edges, path[0], path[max - 1]);
    double pathCost = 1;
    for (size_t i = 0; i<max-1; i++) {
        size_t j = path[i], k = path[i+1];
        rg.addNode(j, inv_label_conversion[j]);
        double cost = A.coeffRef(j, k);
        pathCost *= cost;
        rg.addEdge(j, path[i+1], cost);
    }
    rg.addNode(path[max-1], inv_label_conversion[path[max-1]]);
    return pathCost;
}

void ReadGraph::generateAllPossibleSubgraphsFromPaths(int u, int d, std::vector<bool> &visited, std::vector<size_t> &path, size_t &path_index, double lambda) {
    // Mark the current node and store it in path[]
    visited[u] = true;
    path.emplace_back(u);
    ///path[path_index] = u;
    path_index++;

    // If current vertex is same as destination, then print
    // current path[]
    if (u == d) {
        assert(path_index == path.size());
        std::stringstream  s;
        for (const auto& x : path) {
            s << inv_label_conversion[x];
        }
        pathNameList.emplace_back(s.str());
        generatePathEmbedding(path, path_index, lambda);
    }
    else // If current vertex is not destination
    {
        for (Eigen::SparseMatrix<double, Eigen::RowMajor>::InnerIterator it(A,u); it; ++it)
        {
            assert(it.row() == u);
            size_t dst = it.col();
            if (!visited[dst])
                generateAllPossibleSubgraphsFromPaths(dst, d, visited, path, path_index, lambda);
        }
    }

    // Remove current vertex from path[] and mark it as unvisited
    path_index--;
    path.pop_back();
    visited[u] = false;
}

void ReadGraph::removeNode(size_t k) {
    for (Eigen::SparseMatrix<double, Eigen::RowMajor>::InnerIterator it(A,k); it; ++it)
        it.valueRef() = 0.0;
    inv_label_conversion.erase(k);
}

#include <set>

std::unordered_map<std::string, Eigen::VectorXd> ReadGraph::generateEmbeddings(std::set<std::pair<std::string, std::string>> &k) {
    size_t i = 0;
    std::unordered_map<std::string, Eigen::VectorXd> result;
    ///std::set<std::pair<std::string, std::string>> k;
    if (k.empty()) // Either using the exterior embedding, or calculate its own embedding space
        extractEmbeddingSpace(k);
    size_t j = 0;
    for (const auto& x: decomposedEmbedding) {
        Eigen::VectorXd embedding(k.size());
        size_t i = 0;
        for (const auto& cp : k) {
            auto it = x.find(cp);
            embedding[i] = it == x.end() ? 0 : it->second;
            i++;
        }
        result.emplace(pathNameList[j++], (embedding));
    }
    return result;
}

void ReadGraph::extractEmbeddingSpace(std::set<std::pair<std::string, std::string>> &k) const {
    for (const auto& x : decomposedEmbedding) {
        for (const auto& e : x) {
            k.insert(e.first);
        }
    }
}

void ReadGraph::pushGraphEmbedding() {
    decomposedEmbedding.emplace_back(embedding);
    pathNameList.emplace_back("*");
}
