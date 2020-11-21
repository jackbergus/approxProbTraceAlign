/*
 * Regex.h
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
// Created by giacomo on 18/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_REGEX_H
#define FUZZYSTRINGMATCHING2_REGEX_H

enum Regex_cases {
    BinaryAlternative,
    Consequent,
    Plus,
    Star,
    Optional,
    Paren,
    String,
    NONE
};


#include <vector>
#include <cassert>
#include <memory>
#include <ostream>

template <typename T>
class Regex {
    double cost;
    Regex<T> *left, *right;
    T label;
    Regex_cases casus;

public:
    Regex() : cost{1.0}, left{nullptr}, right{nullptr}, casus{NONE} {}
    Regex(Regex* k) : left{k}, casus{Paren} {}
    Regex(T label, Regex* k, Regex_cases c = Paren, double cost = 1.0) : cost{cost}, label{label}, casus{c}, left{(k)} {}
    Regex(T label) : cost{cost}, label{label}, casus{String}, left{nullptr}, right{nullptr} {}
    Regex(Regex* left, Regex* right = nullptr, double cost = 1.0, Regex_cases c = BinaryAlternative) : cost{cost}, left{(left)}, right{(right)}, casus{c} {}

    Regex_cases cases() const  { return casus; }
    const T& getLabel() const  { return label; }
    Regex<T>* getLeft() const  { return left;  }
    Regex<T>* getRight() const { return right;  }
    double    getCost() const  { return cost;   }

    ~Regex() {
        delete left;
        delete right;
    }

    friend std::ostream &operator<<(std::ostream &os, const Regex &regex) {
        switch (regex.casus) {
            case BinaryAlternative: {
                os << '(' << *regex.left << ") | ";
                if (regex.cost != 1.0) os << regex.cost;
                os << '(' << *regex.right << ")";
                break;
            }
            case String: {
                os << regex.label;
                break;
            }
            case Consequent: {
                os << '(' << *regex.left << ")  ";
                if (regex.cost != 1.0) os << regex.cost;
                os << "  (" << *regex.right << ")";
                break;
            }
            case Plus: {
                os << '(' << *regex.left << ")";
                if (regex.cost != 1.0) os << regex.cost;
                os << " +";
                break;
            }
            case Star: {
                os << '(' << *regex.left << ")";
                if (regex.cost != 1.0) os << regex.cost;
                os << " *";
                break;
            }
            case Optional: {
                os << '(' << *regex.left << ")";
                if (regex.cost != 1.0) os << regex.cost;
                os << " ?";
                break;
            }
            case Paren: {
                os << '(' << *regex.left << ')';
                break;
            }
        }
        return os;
    }

};




#endif //FUZZYSTRINGMATCHING2_REGEX_H
