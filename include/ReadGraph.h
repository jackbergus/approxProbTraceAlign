//
// Created by giacomo on 15/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_READGRAPH_H
#define FUZZYSTRINGMATCHING2_READGRAPH_H

#include <iostream>

#include <Eigen/Sparse>
#include <unordered_map>
#include <functional>
#include <set>
#include <matrix_graph_path/ConditionalReadGraphIterable.h>
#include "Iterator.h"
#include "matrix_graph_path/PathIterator.h"

template <typename T> void matrix_iterator(Eigen::SparseMatrix<double, Eigen::RowMajor>& A, std::unordered_map<size_t, std::string>& map, T& obj) {
    for (int k=0; k < A.outerSize(); ++k)
    {
        for (Eigen::SparseMatrix<double, Eigen::RowMajor>::InnerIterator it(A,k); it; ++it)
        {
            size_t i = it.row(), j = it.col();
            double v = it.value();
            obj.accept(map[i], map[j], v);
        }
    }
}



struct ReadGraph {
    size_t nodes = -1;
    size_t edges = -1;
    size_t source = -1;
    size_t target = -1;
    bool error = false;
    double weight = 1.0;
    Eigen::SparseMatrix<double, Eigen::RowMajor> A;
    std::unordered_map<size_t, std::string> inv_label_conversion;
    std::vector<std::unordered_map<std::pair<std::string, std::string>, double, pair_hash>> decomposedEmbedding;
    std::unordered_map<std::pair<std::string, std::string>, double, pair_hash> embedding;
    typedef Eigen::Triplet<double> T;
    std::vector<std::string> pathNameList;
    std::vector<T> tripletList;

    ReadGraph() = default;
    ReadGraph(const ReadGraph&) = default;
    ReadGraph& operator=(const ReadGraph&) = default;

    /**
     * Reads the graph from a text file.
     * @param filename  File from which the graph will be read
     */
    ReadGraph(const std::string& filename);

    static ReadGraph fromString(const std::string& string, double stringWeight) {
        assert(string.size() > 1);
        ReadGraph rg;
        size_t n = string.size();
        rg.init(n, n-1, 0, n-1);
        for (size_t i = 0; i<n-1; i++) {
            rg.addNode(i, std::string{string[i]});
            rg.addEdge(i, i+1, 1.0);
        }
        rg.addNode(n-1, std::string{string[n-1]});
        rg.finalizeEdgesMatrix(stringWeight);
        return rg;
    }

    void printGraph() const;

    ////////////////////// Matrix iterations



    ///////////////////// Initializations

    /**
     * Initialization of some initial values
     * @param vSize
     * @param eSize
     * @param src
     * @param tgt
     */
    void init(size_t vSize, size_t eSize, size_t src, size_t tgt);

    /**
     * Adds a id-label association to the graph
     * @param id        node id
     * @param label     label id
     * @return Whether the node was correctly inserted (true) or if the node was already inserted (false)
     */
    bool addNode(size_t id, const std::string& label);

    /**
     * Adds an edge to the matrix (bulk insertion)
     * @param src       node src
     * @param dst       node dst
     * @param weight    edge weight, src->dst
     */
    void addEdge(size_t src, size_t dst, double weight);

    /**
     * Finalizes the loaded matrix
     * @param cost  Weight associated to the current Graph
     */
    void finalizeEdgesMatrix(double cost);

    //////////////////////////////// Operations

    /**
     * Generates an embedding for each possible path within the graph
     *
     * @param lambda    Decay factor for penalizing matches with transitive closures
     */
    void generateDecomposedEmbeddings(double lambda);

    /**
     * Decomposes the paths starting from the outgoing edges of the current source path. This should be done when
     * the first node is considered as the only tau in the graph (I can always assume that, for the remaining, we could
     * do a transitive closure)
     *
     * @param decomposition  Graph decomposition (the source node is updated to each adj node of the source nodes)
     */
    void decomposeStart(double lambda);

    /**
     * Generates an embedding for the whole graph, without any decomposition
     *
     * @param lambda    Transitive closure decay factor
     */
    void generateEmbeddings(double lambda);

    //Print the embedding associated to the single graph, if any
    void printLocalEmbedding();

    //Print the decomposed paths, if generated
    void printAllPathsEmbeddings();

    std::unordered_map<std::string, Eigen::VectorXd> generateEmbeddings(std::set<std::pair<std::string, std::string>> &k);

    void extractEmbeddingSpace(std::set<std::pair<std::string, std::string>> &k) const;

    void pushGraphEmbedding();

    /**
     * Returns the java-friendly interface to iterators (hasNext && next)
     *
     * @param doNotVisitLoopsTwice      Whether I want to traverse the node only once or not
     * @param maxPathLength             Maximum path length that I want to visit
     * @param minimumPathCost           Minimum admissible cost for the path
     * @return                          iterator
     */
    PathIterator javaPathIterator(bool doNotVisitLoopsTwice = true, size_t maxPathLength = std::numeric_limits<size_t>::max(),
                                  const double minimumPathCost = 2.0 * std::numeric_limits<double>::epsilon());


    /**
     * Iterate over the paths as an iterable
     *
     * @param doNotVisitLoopsTwice      Whether I want to traverse the node only once or not
     * @param maxPathLength             Maximum path length that I want to visit
     * @param minimumPathCost           Minimum admissible cost for the path
     * @return                          Iterable
     */
    ConditionalReadGraphIterable iterateOverPaths(bool doNotVisitLoopsTwice = true, size_t maxPathLength = std::numeric_limits<size_t>::max(),
                                                  const double minimumPathCost = 2.0 * std::numeric_limits<double>::epsilon());



private:

    /**
     * Prints an embedding
     * @param embedding
     */
    void print(const std::unordered_map<std::pair<std::string, std::string>, double, pair_hash>& embedding);

    /**
     * Generates the embedding for the graph's path
     *
     * In order to do so, the path is selected as a subgraph: such graph is loaded, and then the embedding for the graph
     * is computed
     *
     * @param path
     * @param max           Size limit
     * @param lambda        Decay factor while traversing the path
     */
    void generatePathEmbedding(std::vector<size_t>& path, size_t max, double lambda);
    void generateAllPossibleSubgraphsFromPaths(int u, int d, std::vector<bool>& visited,
                                               std::vector<size_t>& path, size_t& path_index, double lambda);



    void generateAllPossiblePathsRecursively2(int u, int d, std::vector<bool> &visited, std::vector<size_t> &path, size_t &path_index, double lambda, std::vector<std::vector<size_t>>& result) {
        // Mark the current node and store it in path[]
        visited[u] = true;
        path.emplace_back(u);
        ///path[path_index] = u;
        path_index++;

        // If current vertex is same as destination, then print
        // current path[]
        if (u == d) {
            assert(path_index == path.size());
            result.emplace_back(path);
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

    void removeNode(size_t i);

    double insertPath(const std::vector<size_t> &path, size_t max, ReadGraph &rg);

    template <typename T> void printStream(T& obj) {
        obj << "nodes: " << nodes << std::endl;
        obj << "edges: " << edges<< std::endl;
        obj << "source: " << source<< std::endl;
        obj << "target: " << target<< std::endl;
        obj << "weight: " << weight<< std::endl;
        assert(inv_label_conversion.size() == nodes);
        for (const auto& cp : inv_label_conversion)
            obj << cp.first << " " << cp.second<< std::endl;
        for (int k=0; k < A.outerSize(); ++k)
        {
            for (Eigen::SparseMatrix<double, Eigen::RowMajor>::InnerIterator it(A,k); it; ++it)
            {
                obj << it.row() << " " << it.col() << " " << it.value() << std::endl;
            }
        }
    }



};



#endif //FUZZYSTRINGMATCHING2_READGRAPH_H
