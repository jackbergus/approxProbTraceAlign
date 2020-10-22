//
// Created by giacomo on 21/10/20.
//

#ifndef FUZZYSTRINGMATCHING2_POINTCLASS_H
#define FUZZYSTRINGMATCHING2_POINTCLASS_H


struct PointClass {
    double cost;
    double precision;
    bool   isCostSimilarityOtherwise;

    PointClass(double cost, double precision, bool isCost) : cost(cost), precision(precision), isCostSimilarityOtherwise{isCost} {}
    PointClass() : cost{0.0}, precision{1.0}, isCostSimilarityOtherwise{true} {};
    PointClass(const PointClass&) = default;
    PointClass& operator=(const PointClass&) = default;

    static double costToSimilarity(double x) { return 1.0/(x/5.0+1.0); }
    static double similarityToCost(double x) { return 5*(1.0/x-1.0); }

    double getWeightedCost() const { return (isCostSimilarityOtherwise ? cost : PointClass::similarityToCost(cost)) * precision; };
    double getWeightedSimilarity() const { return (isCostSimilarityOtherwise ? PointClass::costToSimilarity(cost) : cost) * precision; };
    double getSimilarityRanking() const { return getWeightedSimilarity(); };
    [[nodiscard]] PointClass asSimilarityVector() const {
        return isCostSimilarityOtherwise ? PointClass{costToSimilarity(cost), precision, false} : *this;
    }
    [[nodiscard]] PointClass asCostVector() const {
        return !isCostSimilarityOtherwise ? PointClass{similarityToCost(cost), precision, true} : *this;
    }
};


#endif //FUZZYSTRINGMATCHING2_POINTCLASS_H
