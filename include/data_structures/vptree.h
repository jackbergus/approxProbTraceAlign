/*
 * vptree.h
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
// Created by giacomo on 07/12/2019.
//

#ifndef TUTORIALS_VPTREE_NODE_H
#define TUTORIALS_VPTREE_NODE_H

#include <vector>
#include <random>
#include <algorithm>
#include <iostream>


/**
 * A kernel function provides the similarity between two elements
 * @tparam T
 */
template <typename T> struct DistanceFunction {
    DistanceFunction() = default;
    virtual ~DistanceFunction() = default;
    virtual double distance(const T& lhs, const T& rhs) {
        return (&lhs == &rhs);
    }
};


template <typename Point> struct vp_node {

    unsigned int id; ///<@ id associated to the current node (i.e., insertion order)
    Point pt;        ///<@ value to be compared
    double radius;   ///<@ radius that, in the standard definition, provides the boundary between the left and right nodes
    unsigned int leftChild = std::numeric_limits<unsigned int>::max(); ///<@ setting the leftChild to nullptr
    unsigned int rightChild = std::numeric_limits<unsigned int>::max(); ///<@ setting the rightChild to nullptr
    bool isLeaf = false; ///<@ nevertheless, each node is not necessairly a leaf

    vp_node(unsigned int id, Point pt, double radius) : id(id), pt(pt), radius(radius) {}
};

template <typename Point, typename DistanceFunction> struct vp_tree {
    std::vector<vp_node<Point>> tree; ///<@ representing the tree as a vector
    DistanceFunction ker;             ///<@ instance of the distance function provided by the template parameter
    std::mt19937 rng;                 ///<@ random number generator

    // Please note: this is not a thread-safe implementation
    double tau;                         ///<@ current distance decision value at the iteration
    std::set<Point, DistanceFunction> found;          ///<@ current found elements for the given lookup query

    /**
     * Initializing the element with a vector of elements
     * @param ls
     */
    vp_tree(const std::vector<Point>& ls) {
        tree.reserve(ls.size());
        size_t i = 0;
        for (auto x : ls)
            tree.emplace_back(i++, x, 0);
        recursive_restruct_tree(0, ls.size()-1);
    }

    /**
     * Initializing the tree with some static values
     * @param ls
     */
    vp_tree(std::initializer_list<Point> ls) {
        tree.reserve(ls.size());
        size_t i = 0;
        for (auto x : ls)
            tree.emplace_back(i++, x, 0);
        recursive_restruct_tree(0, ls.size());
    }

    /**
     * Looking for the elements near to id that are not exactely the same as him
     * @param id
     */
    void topkSearch(const Point& id, size_t k) {
        found.clear();
        tau    = std::numeric_limits<double>::max();
        ///std::cout << "Id. pt = " << tree[id].pt << std::endl;
        lookUpNearsetTo(0, id, k);
    }

private:
    void lookUpNearsetTo(size_t root_id, const Point& id, size_t k) {
        //std::cout << root_id << std::endl;
        ///auto node = tree[id];
        auto root = tree[root_id];
        double rootRadius = tree[root_id].radius;
        double dist = ker(root.pt, id);

        ///std::cout << "---------" << std::endl;
        ///std::cout << std::string(depth, '.') <<dist << "=\\delta(" << root.pt << "," << node.pt << ")" << std::endl;
        ///std::cout << std::string(depth, '.') << "Root. Pt = " << root.pt << " radius = " << rootRadius << " rootDistance = " << dist << std::endl;
        /*if ((root_id != id))*/ { // do not add the same object

            found.insert(root.pt);
            if (found.size() > k) {
                found.erase(std::prev(found.end()));
            }
#if 0
            // If I find a better object, then discard all the previous ones, and set the current one
            if (definitelyLessThan(dist,tau)) {
                found.clear();
                found.emplace_back(root_id);
                ///std::cout << std::string(depth, '.') << "New Tau pt: " << root.pt << std::endl;
                ///std::cout << std::string(depth, '.') << "(tau=dist): " << dist << std::endl;
                tau = dist;
            } else
                // Otherwise, if they are very similar, then I could add this other one too
            if (approximatelyEqual(dist, tau)) {
                found.emplace_back(root_id);
                tau = std::min(dist, tau);
                ///std::cout << std::string(depth, '.') << "Old Tau " << tau << "=" << dist << std::endl;
            }
#endif
        }

        // Continuing with the traversal depending on the distance from the root
        if (dist < rootRadius) {
            ///std::cout << std::string(depth, '.')  << dist << '<' << rootRadius << std::endl;
            if (root.leftChild != std::numeric_limits<unsigned int>::max() && dist - tau <= rootRadius) {
                ///std::cout << std::string(depth, '.') << dist << '-' << tau << "<=" << rootRadius <<  std::endl;
                lookUpNearsetTo(root.leftChild, id, k);
            }

            // At this stage, the tau value might be updated from the previous recursive call
            if (root.rightChild != std::numeric_limits<unsigned int>::max() && dist + tau >= rootRadius) {
                ///std::cout << std::string(depth, '.') << dist << '+' << tau << ">=" << rootRadius <<  std::endl;
                lookUpNearsetTo(root.rightChild, id, k);
            }
        } else {
            ///std::cout << std::string(depth, '.') << dist << '<' << rootRadius << std::endl;
            if (root.rightChild != std::numeric_limits<unsigned int>::max() && dist + tau >= rootRadius) {
                ///std::cout << std::string(depth, '.') << dist << '+' << tau << ">=" << rootRadius <<  std::endl;
                lookUpNearsetTo(root.rightChild, id, k);
            }

            // At this stage, the tau value might be updated from the previous recursive call
            if (root.leftChild != std::numeric_limits<unsigned int>::max() && dist - tau <= rootRadius) {
                ///std::cout << std::string(depth, '.') << dist << '-' << tau << "<=" << rootRadius <<  std::endl;
                lookUpNearsetTo(root.leftChild, id, k);
            }
        }
    }

    void recursive_restruct_tree(size_t first, size_t last) {
        if (first >= last) {
            // If the elements overlaps, then I reached a leaf node
            tree[first].isLeaf = true;
            tree[first].leftChild = std::numeric_limits<unsigned int>::max();
            tree[first].rightChild = std::numeric_limits<unsigned int>::max();
        } else {
            if ((last - first) <= 1) {
                // If the elements differ by two, then I decide that one is the root, and the other is the child
                tree[first].radius = ker(tree[first].pt, tree[last].pt);
                tree[first].leftChild = last;
                tree[first].rightChild = std::numeric_limits<unsigned int>::max();
            } else {
                // Picking the root randomly as the first element of the tree ~ O(1)
                std::uniform_int_distribution<int> uni(first, last - 1);
                int root = uni(rng);
                std::swap(tree[first], tree[root]);

                size_t median = (first + last) / 2; // TODO: other heuristic, separating the elements within a fixed radius from the object, and the ones out

                /*
                 * nth_element is a partial sorting algorithm that rearranges elements in [first, last) such that:
                 *
                 * - The element pointed at by median is changed to whatever element would occur in that position if [first, last) were sorted.
                 * - All of the elements before this new nth element are less than or equal to the elements after the new nth element.
                 */
                std::nth_element(
                        tree.begin() + first + 1,//first
                        tree.begin() + median,   //median
                        tree.begin() + last,    //last
                        [first, this] (const vp_node<Point>& i1, const vp_node<Point>& i2) {
                            return ker(tree[first].pt, i1.pt) < ker(tree[first].pt, i2.pt);
                        });

                // Setting the separating elements
                tree[first].radius = ker(tree[first].pt, tree[median].pt);

                // Recursively splitting in half the elements within my radius and the ones out
                tree[first].leftChild = first+1;
                tree[first].rightChild = (first + last) / 2 + 1;
                recursive_restruct_tree(tree[first].leftChild, tree[first].rightChild-1);
                recursive_restruct_tree(tree[first].rightChild, last);

            }
        }
    }
};

#endif //TUTORIALS_VPTREE_NODE_H
