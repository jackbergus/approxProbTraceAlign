//
// Created by giacomo on 20/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_RANKING_H
#define FUZZYSTRINGMATCHING2_RANKING_H

#include <unordered_set>
#include <unordered_map>
#include <map>
#include <cmath>
#include <vector>
#include <matrix_graph_path/path_info.h>
#include <distances/strings/Levenstein.h>
#include <cassert>

template <typename BenchmarkKey> struct Ranking {
    Ranking() {}
    Ranking(const Ranking&) = default;
    Ranking&operator=(const Ranking&) = default;
    ~Ranking() {}

    void addScore(const BenchmarkKey& element, double score) {
        assert(scoring.insert(std::make_pair(element, score)).second);
        orderedList[score].emplace_back(element);
        localS.insert(element);
        summation += score;
        size += 1.0;
    }

    double score(const BenchmarkKey& element, double defaultValue) const  {
        auto it = scoring.find(element);
        return it == scoring.end() ? defaultValue : it->second;
    }

    double SpearmanCorrelationIndex(const Ranking<BenchmarkKey>& ranking, double defaultValue) {
        std::vector<BenchmarkKey> S;
        auto begin = localS.size()>=ranking.localS.size() ? localS.begin() : ranking.localS.begin(),
                end = localS.size() >= ranking.localS.size() ? localS.end() : ranking.localS.end();
        auto& set = localS.size() >= ranking.localS.size()  ? ranking.localS : localS;

        for (; begin != end; begin++) {
            if (set.contains(*begin)) S.emplace_back(*begin);
        }

        std::set_intersection(localS.begin(), localS.end(), ranking.localS.begin(), ranking.localS.end(), std::back_inserter(S));
        //S.insert(ranking.localS.begin(), ranking.localS.end());
        double average = summation / size;
        double aAverage = ranking.summation / ranking.size;
        double over = 0.0;
        double lhsSquaredSum = 0.0, rhsSquaredSum = 0.0;

        for (const auto& x : S) {
            double lhs = (score(x, defaultValue) - average);
            double rhs = (ranking.score(x, defaultValue) - aAverage);
            over += lhs * rhs;
            lhsSquaredSum += std::pow(lhs, 2.0);
            rhsSquaredSum += std::pow(rhs, 2.0);
        }
        return over / std::sqrt(lhsSquaredSum * rhsSquaredSum);
    }

    const typename std::map<double, std::vector<BenchmarkKey>>::const_reverse_iterator & maximum() const  {
        return orderedList.crbegin();
    }

    void mergeRanks(const Ranking<BenchmarkKey>& other)  {
        localS.insert(other.localS.begin(), other.localS.end());
        orderedList.clear();
        for (const auto& cp : other.scoring) {
            auto it = scoring.insert({cp.first, cp.second});
            if (!it.second) it.first->second = std::max(it.first->second, cp.second);
        }
        for (const auto& cp: scoring) {
            orderedList[cp.second].emplace_back(cp.first);
        }
    }

    size_t getMaximumRank() const { return orderedList.size(); }
    size_t getOutOfRankingPos() const { return orderedList.size()+1; }

    /**
     * Returns the position within the range 1..L+1, where L is the maximum expected rank and L+1 represents the out-of-rank
     *
     * @param element Element to be ranked
     *
     * @return Ranking value
     */
    size_t getRanking(const BenchmarkKey& element) const {
        double expected = scoring.at(element);
        size_t pos = 1;
        for (auto it = orderedList.rbegin(), en = orderedList.rend(); it != en; it++) {
            if (it->first == expected) return pos;
            else pos++;
        }
        return pos;
    }

    /**
     * Given the element that we expect to be the first, we return the ranking precision as 1- (getRank(x)-1)/l
     *
     * @param expectedFirst     Element that we expect to be the best possible
     *
     * @return  Precision associated to predicting expectedFirst as first element
     */
    double getRankingPrecision(const BenchmarkKey& expectedFirst) const {
        double l = orderedList.size();
        return 1.0 - ((double)getRanking(expectedFirst) - 1.0) / l;
    }


    /*const path_info &left, const path_info &right) const {
    return GeneralizedLevensteinDistance(left.path, right.path);*/
    //template <typename SimilarityFunction>
    static double rankingDistance(const struct Ranking<BenchmarkKey>& lhs, const struct Ranking<BenchmarkKey>& rhs, const std::function<double(const BenchmarkKey&, const BenchmarkKey&)>& function) {//leftMap(cpl.first), rightMap(cpr.first)
        double sum = 0.0;
        for (const auto& cpl : lhs.scoring) {
            for (const auto& cpr : rhs.scoring) {
                double maxScore = (cpl.second * cpr.second);
                double similarity = 1.0 / (1.0 + function(cpl.first, cpl.first) / 5.0);
                sum += (maxScore * similarity);
            }
        }
        return sum;
    }

    //template <typename SimilarityFunction>
    double normalizedRank(const struct Ranking<BenchmarkKey>& rhs, const std::function<double(const BenchmarkKey&, const BenchmarkKey&)>&  function) {
        return rankingDistance(*this, rhs, function) / std::sqrt(rankingDistance(*this, *this, function) * rankingDistance(rhs, rhs, function));
    }

    Ranking<BenchmarkKey> topK(size_t topK) {
        Ranking<BenchmarkKey> toReturn;
        for (auto it = orderedList.rbegin(), en = orderedList.rend(); it != en; it++) {
            for (const auto& x : it->second) {
                toReturn.addScore(x, it->first);
            }
            if (toReturn.size >= topK) break;
        }
        return toReturn;
    }

    Ranking<BenchmarkKey> topK(size_t topK, std::function<double(double)>& f) {
        Ranking<BenchmarkKey> toReturn;
        for (auto it = orderedList.rbegin(), en = orderedList.rend(); it != en; it++) {
            for (const auto& x : it->second) {
                toReturn.addScore(x, f(it->first));
            }
            if (toReturn.size >= topK) break;
        }
        return toReturn;
    }

private:
    std::unordered_set<BenchmarkKey> localS;
    std::map<double, std::vector<BenchmarkKey>> orderedList;
    std::unordered_map<BenchmarkKey, double> scoring;
    double summation = 0;
    double size = 0;
};



#endif //FUZZYSTRINGMATCHING2_RANKING_H
