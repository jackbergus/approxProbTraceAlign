//
// Created by giacomo on 16/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_RANKINGANALYSISTYPE_H
#define FUZZYSTRINGMATCHING2_RANKINGANALYSISTYPE_H

enum RankingAnalysisType {
    RankingDistance,        // Computes the distance between the two rankings considered as weighted sets. So,
                            // each element is associated to a score and the distance between the two sets is defined
                            // as the distance between all the elemenents of the sets, normed by the similarity between
                            // the elements

    SpearmanCorrelationIndex, // Computes the Spearman Correlation Index between the two proposed rankings. This
                              // benchmark is mainly done for comparing the Spearman Correlation Index with the
                              // proposed ranking similarity metric, the MinimumEditMaximumSubstring

    MinimumEditMaximumSubstring, // Proposed ranking for comparing two different similarity elements

};

#endif //FUZZYSTRINGMATCHING2_RANKINGANALYSISTYPE_H
