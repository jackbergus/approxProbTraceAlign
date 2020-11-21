//
// Created by giacomo on 16/11/20.
//

#include <topk/algorithm_map.h>

void rectify(struct algorithm_map& x) {}
void rectify2(struct algorithm_map& x) {
    size_t N = x.edits.size();
    if ((N>=2) && ((x.edits[N-1] == x.edits[N-2]))) {  // Removing the clashes of slight changes in the order. Nevertheless, I will analyse the monotonicity, so this information won't be discarded
        x.edits.pop_back();
    }
}

#include "benchmarking/minimum_edit_maximum_substring.h"
#include <numeric>
#include <unordered_map>

double minimum_edit_maximum_substring(const std::vector<size_t> &ranking) {

    /*std::cout << "{{{INPUT: ";
    for (const size_t& x : ranking)
        std::cout << x << ", ";
    std::cout << "}}}a" << std::endl;*/

    std::vector<algorithm_map> backup;

    std::vector<size_t> expected(ranking.size());
    std::iota(expected.begin(), expected.end(), 1);
    double S = ranking.size();

    {
        std::unordered_map<size_t, algorithm_map> C;
        C[0] = {};
        C[0].index = 0;
        for (size_t i = 0, N = ranking.size(); i<N; i++) {
            size_t current  = ranking[i];
            if ((i == 0) || (ranking[i] > ranking[i-1])) {
                for (auto& kv : C) kv.second.sequence.emplace_back(ranking[i]);
                for (size_t j = ranking[i-1]+1, M = ranking[i]-1; j<M; j++) {
                    for (auto& kv : C) kv.second.edits.emplace_back(j);
                }
                for (auto& kv : C) rectify(kv.second);
            } else {
                for (const auto& kv : C) backup.emplace_back(kv.second);
                for (auto& kv: C) {
                    kv.second.edits.emplace_back(ranking[i]);
                    kv.second.sequence.emplace_back(ranking[i]);
                    rectify2(kv.second);
                }
                C[i] = {};
                C[i].index = i;
                C[i].sequence.emplace_back(ranking[i]);
            }
        }
        for (const auto& kv : C) backup.emplace_back(kv.second);
    }

    double max = -1; long long int index = -1;
    for (size_t i = 0, N = backup.size(); i<N; i++) {
        algorithm_map& ref = backup[i];

        // /////////////////////////////////////////////////////////
        // Discard the sequence if it is prevalently decrementing //
        // /////////////////////////////////////////////////////////
        long long int summation = 0;
        for (size_t j = 1, M = ref.sequence.size(); j<M; j++) {
            long long int difference = (((long long int)ref.sequence[j])-((long long int)ref.sequence[j-1]));
            if ((difference < 0) && (summation > 0)) summation = 0;
            summation += (((long long int)ref.sequence[j])-((long long int)ref.sequence[j-1]));
            if ((summation < 0) && (difference>0)) break;
        }
        if (summation <= 0) continue;
        // /////////////////////////////////////////////////////////
        // /////////////////////////////////////////////////////////
        // /////////////////////////////////////////////////////////

        ref.precision = ((double)ref.sequence.size())/S;
        ref.simialrity = 1.0/(((double)ref.edits.size())/5.0+1.0);
        ref.score = ref.precision * ref.simialrity;
        if ((ref.score > max ) || ((ref.score == max) && (ref.simialrity > backup[index].simialrity)) ||((ref.score == max) && (ref.simialrity == backup[index].simialrity) && (ref.precision > backup[index].precision)) ) {
            max = ref.score;
            index = i;
        }
    }


    if ((max < 0) || (index == -1)) {
        ////std::cout<< "There is no good sequence" << std::endl;
        return 0;
    }
    /*std::cout << "Score " << backup[index].score << " for: ";
    for (const auto& sol : backup[index].sequence)
        std::cout << sol << ", ";
    std::cout << "from index " << index <<  std::endl;*/
    return backup[index].score;

}
