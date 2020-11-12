//
// Created by giacomo on 11/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_WEIGHTESTIMATOR_H
#define FUZZYSTRINGMATCHING2_WEIGHTESTIMATOR_H

#include <GenericGraph.h>
#include <utils/SimpleTraceFromFile.h>
#include <unordered_map>

namespace spd_we {

    enum WeightEstimatorCases {
        W_FREQ,
        W_LHPAIR,
        W_RHPAIR,
        W_PAIRSCALE,
        W_FORK,
        W_CONSTANT
    };


    /**
     * Implemeting the paper by Adam Burke et al. "Stochastic Process Discovery By Weight Estimation"
     * @tparam id_type
     */
    template <typename id_type>
    class WeightEstimator {
        GenericGraph<id_type>* graph;
        std::unordered_map<id_type, double> pw, freq;
        std::unordered_map<std::string, double> qi,qf;
        std::unordered_map<std::pair<std::string, std::string>, double, pair_hash> pq;
        const std::vector<Transaction<std::string>>* log;

        void prepareForkEstimator(const std::vector<Transaction<std::string>>& log) {
            if (this->log == &log) return; // It has already been prepared!
            else this->log = (&log);//const_cast<std::vector<Transaction<std::string>> *>
            auto start = graph->getStart();
            pw.clear();
            freq.clear();
            std::set<std::string> labels;
            for (const auto& u : graph->getNodes()) {
                if (u == start) {
                    pw.emplace(u, std::max(1.0, (double)log.size()));
                } else {
                    double S = 0.0;
                    for (const auto& s : graph->ingoing(u)) {
                        std::string label_s = graph->getNodeLabel(s.first);
                        for (const auto& t : graph->outgoing(u)) {
                            std::string label_t = graph->getNodeLabel(t.first);
                            auto key = std::make_pair(label_s, label_t);
                            auto it = pq.find(key);
                            if (it != pq.end()) {
                                S += it->second;
                            } else {
                                double pqc = q_P(log, label_s, label_t);
                                pq.emplace(std::make_pair(label_s, label_t), pqc);
                                S += pqc;
                            }
                        }
                    }
                    pw.emplace(u, std::max(S, 1.0));
                }
                std::string ulabel = graph->getNodeLabel(u);
                labels.insert(ulabel);
                freq.emplace(u, w_freq(log, ulabel));
            }

            std::pair<std::string, std::string> cp;
            for (const auto& label_s : labels) {
                cp.first = label_s;
                qi.emplace(label_s, q_I(log, label_s));
                qf.emplace(label_s, q_F(log, label_s));
                for (const auto& label_t : labels) {
                    cp.second = label_t;
                    auto it = pq.find(cp);
                    if (it == pq.end()) {
                        pq.emplace(std::make_pair(label_s, label_t), q_P(log, label_s, label_t));
                    }
                }
            }
        }

    public:
        WeightEstimator() { graph = nullptr; log = nullptr; }
        void setGraph(GenericGraph<id_type> *graph) {
            WeightEstimator::graph = graph;
        }

        void setLog(const std::vector<Transaction<std::string>>& log) {
            prepareForkEstimator(log);
        }

        double getNodeWeight(const id_type& id, WeightEstimatorCases casus) {
            std::string label = graph->getNodeLabel(id);
            std::pair<std::string, std::string> key_pair;
            switch (casus) {
                case W_CONSTANT: {
                    return 1.0;
                }

                case W_LHPAIR: {
                    key_pair.second = label;
                    double S = 0.0;
                    for (const auto& in : graph->ingoing(id)) {
                        for (const auto& in2 : graph->ingoing(in.first)) {
                            std::string label2 = graph->getNodeLabel(in2.first);
                            key_pair.first = label2;
                            S += pq.at(key_pair);
                        }
                    }
                    return std::max(1.0, qi.at(label)+qf.at(label)+S);
                }

                case W_RHPAIR: {
                    key_pair.first = label;
                    double S = 0.0;
                    for (const auto& in : graph->outgoing(id)) {
                        for (const auto& in2 : graph->outgoing(in.first)) {
                            std::string label2 = graph->getNodeLabel(in2.first);
                            key_pair.second = label2;
                            S += pq.at(key_pair);
                        }
                    }
                    return std::max(1.0, qi.at(label)+qf.at(label)+S);
                }

                case W_PAIRSCALE: {
                    double T = graph->nodes();
                    double L = log->size();
                    double LT = L/T;

                    key_pair.first = label;
                    double S = 0.0;
                    for (const auto& in : graph->outgoing(id)) {
                        for (const auto& in2 : graph->outgoing(in.first)) {
                            std::string label2 = graph->getNodeLabel(in2.first);
                            key_pair.second = label2;
                            S += pq.at(key_pair);
                        }
                    }
                    double result = ( qi.at(label)+qf.at(label)+S)*T/L;
                    return (result <= std::numeric_limits<double>::epsilon()) ? 0.0 : result;
                }


                case W_FORK: {
                    double S = 0.0;
                    for (const auto& p : graph->ingoing(id)) {
                        double  D = 0.0;
                        for (const auto& tp : graph->outgoing(id)) D += freq.at(tp.first);
                        if (D == 0.0) D = 1.0;
                        S+= (pw.at(p.first) / D);
                    }
                    S *= freq.at(id);
                    return S;
                }

                case W_FREQ:
                default:
                    return freq.at(id);
            }
        }

    };

};

#endif //FUZZYSTRINGMATCHING2_WEIGHTESTIMATOR_H
