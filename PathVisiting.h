//
// Created by giacomo on 19/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_PATHVISITING_H
#define FUZZYSTRINGMATCHING2_PATHVISITING_H

//#include <Generator.hpp>

#include <vector>
#include <cassert>
#include <sstream>
#include <Eigen/Sparse>


/**
 * From Knuth's the art of computer programming
 */
template  <typename T>  bool definitelyLessThan(T a, T b)
{
    return (b - a) > ( (std::abs(a) < std::abs(b) ? std::abs(b) : std::abs(a)) * std::numeric_limits<T>::epsilon());
}

/**
 * From Knuth's the art of computer programming
 */
template <typename T> bool approximatelyEqual(T a, T b)
{
    return std::abs(a - b) <= ( (std::abs(a) < std::abs(b) ? std::abs(b) : std::abs(a)) * std::numeric_limits<T>::epsilon());
}

/**
 * From Knuth's the art of computer programming
 */
template <typename T> bool approximatelyLessEqual(T a, T b)
{
    return definitelyLessThan(a, b) || approximatelyEqual(a, b);
}

#include <CopyableStack.h>
#include <ostream>

struct path_info {
    double      cost;
    std::string path;

    path_info(double cost, const std::string &path) : cost(cost), path(path) {}
    path_info() : cost{0.0}, path{""} {}
    path_info(const path_info&) = default;
    path_info& operator=(const path_info&) = default;

    friend std::ostream &operator<<(std::ostream &os, const path_info &info) {
        os << "cost: " << info.cost << " path: " << info.path;
        return os;
    }
};

#include <unordered_set>
#include <stack>
#include <iostream>

class PathVisiting {

    using MatrixIt = Eigen::SparseMatrix<double, Eigen::RowMajor>::InnerIterator;

    // Iteration aspects
    struct stack_info {
        size_t currentNode;
        size_t destination;
        std::vector<size_t> path;
        double probabilisticPath;
        std::unordered_set<size_t> visited;
        MatrixIt currentIteration;
        bool firstInsertion;

        stack_info() = default;
        stack_info(size_t currentNode, size_t destination, const std::vector<size_t> path, double probabilisticPath,
                   const std::unordered_set<size_t> &visited,
                   Eigen::SparseMatrix<double, Eigen::RowMajor>::InnerIterator currentIteration, bool firstInsertion)
                : currentNode(currentNode), destination(destination), path((path)),
                  probabilisticPath(probabilisticPath), visited(visited), currentIteration(currentIteration),
                  firstInsertion(firstInsertion) {}
        stack_info(const stack_info& x) = default;
        stack_info& operator=(const stack_info&) = default;
        ~stack_info() = default;

        path_info asPathInfo(
                const std::unordered_map<size_t, std::string>* map) const {
            path_info p;
            if (map) {
                std::stringstream  s;
                for (const auto& x : path) {
                    s << map->at(x);
                }
                p.path = s.str();
            }
            p.cost = probabilisticPath;
            return p;
        }
    };


    Eigen::SparseMatrix<double, Eigen::RowMajor>* A;
    std::unordered_map<size_t, std::string>* inv_label_conversion;
    const double minimumPathCost;
    size_t maxPathLength;
    bool doNotVisitLoopsTwice;
    std::stack<stack_info> S;

public:


    PathVisiting(const PathVisiting& x) : minimumPathCost{x.minimumPathCost} {
        A = x.A;
        inv_label_conversion = x.inv_label_conversion;
        maxPathLength = x.maxPathLength;
        doNotVisitLoopsTwice = x.doNotVisitLoopsTwice;
        S = x.S;
    }


    PathVisiting(Eigen::SparseMatrix<double, Eigen::RowMajor> *a,
                 std::unordered_map<size_t, std::string> *invLabelConversion, size_t maxPathLength,
                 const double minimumPathCost = 2.0 * std::numeric_limits<double>::epsilon(),
                 bool visitLoops = false)

            : A(a), inv_label_conversion(invLabelConversion),
              minimumPathCost(minimumPathCost), maxPathLength(maxPathLength == 0 ? a->size()*2 : maxPathLength),
              doNotVisitLoopsTwice(visitLoops), S{} {
        assert(a);
        assert(invLabelConversion);
    }

    PathVisiting& operator=(const PathVisiting&) = default;

    /*void enumeratePath(int currentNode, int destination, std::vector<bool> &visited, std::vector<size_t> &path, double probabilisticPath) {
        if (!visitLoops) visited[currentNode] = true;
        path.emplace_back(currentNode);


        // If current vertex is same as destination, then print
        if ((currentNode == destination) || (approximatelyLessEqual(probabilisticPath, minimumPathCost)) || (maxPathLength == visited.size())) {

        }
        else // If current vertex is not destination
        {
            for (Eigen::SparseMatrix<double, Eigen::RowMajor>::InnerIterator it(A, currentNode); it; ++it)
            {
                assert(it.row() == currentNode);
                size_t dst = it.col();
                if (!visited[dst])
                    enumeratePath(dst, destination, visited, path, probabilisticPath * it.value());
            }
        }

        // Remove current vertex from path[] and mark it as unvisited
        path.pop_back();
        if (!visitLoops) visited[currentNode] = false;
    }*/

    void begin(size_t currNode, size_t destination, double probabilisticPath = 1.0) {
        clear();
        std::unordered_set<size_t> v;
        //v.emplace(currNode);
        Eigen::SparseMatrix<double, Eigen::RowMajor>::InnerIterator empty;
        S.push({currNode, destination, std::vector<size_t>{}, probabilisticPath, v, empty, true});
    }

    bool hasNext() {
        bool val = (!S.empty());
        if (val) val =
                val&&calculateNext();
        if (!val) {
            clear();
        }
        return val;
    }

    path_info next() {
        path_info toRet;
        if (!S.empty()) {
            const auto x = S.top();
            toRet = x.asPathInfo(inv_label_conversion);
            ///if (x.currentNode == x.destination) S.pop();
            S.pop();
        }
        return toRet;
    }

    ~PathVisiting() { clear(); }


private:
    void clear() {
        while (! S.empty()) S.pop();
    }

    bool calculateNext() {
        while (! S.empty()) {
            stack_info& current = S.top();
            if (current.firstInsertion) {
                std::cout << '[' << current.currentNode << ']' << std::endl;
                current.firstInsertion = false;
                if ((current.currentNode == current.destination) || (current.path.size() == maxPathLength)) {
                    std::cout << '[' << current.currentNode << "]!" << std::endl;
                    current.path.emplace_back(current.currentNode);
                    return true;
                } else {
                    if ((!doNotVisitLoopsTwice) || (current.visited.emplace(current.currentNode).second)) {
                        current.currentIteration = MatrixIt(*A, current.currentNode);
                        visitCurrentNeighbourAndMoveNext(current, current.currentIteration);
                    } else {
                        S.pop(); // Skip it: I cannot do more with it
                    }
                }
            } else {
                if (!current.currentIteration) {
                    S.pop(); // Skip it: I cannot do more with it
                } else {
                    // Otherwise, continue to visit to the next node
                    visitCurrentNeighbourAndMoveNext(current, current.currentIteration);
                }
            }

        }
        return false;
    }

    void
    visitCurrentNeighbourAndMoveNext(const stack_info &topStackInformation,
                                     MatrixIt &currentIterationStep)  {
        if (currentIterationStep) {
            ///assert(it.row() == current.currentNode);
            size_t dst = currentIterationStep.col();
            if (canVisit(topStackInformation, currentIterationStep, dst)) {
                std::vector<size_t> v = topStackInformation.path;
                v.emplace_back(topStackInformation.currentNode);
                S.push({dst, topStackInformation.destination, v, topStackInformation.probabilisticPath * currentIterationStep.value(), topStackInformation.visited, {}, true});
            }
            ++currentIterationStep;
        }
    }

    bool
    canVisit(const stack_info &current, const Eigen::SparseMatrix<double, Eigen::RowMajor>::InnerIterator &it, size_t dst) const {
        return ( // Both the following conditions must follow:
                (!doNotVisitLoopsTwice) || current.visited.find(dst) == current.visited.end()) && // 1) Both I can visit a node if and only if it has not visited before or if this condition is not required,
        (approximatelyLessEqual(minimumPathCost, current.probabilisticPath * it.value())); // 2) And the path cost must worth it

    }


};


#endif //FUZZYSTRINGMATCHING2_PATHVISITING_H
