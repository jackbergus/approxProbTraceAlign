//
// Created by giacomo on 13/11/20.
//

#include "log/log_operations.h"
#include <random>
#include <iostream>


void performLogOperation(const std::vector<LogOperationConfiguration> &vconf,
                         std::vector<Transaction<std::string>> &log) {

    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_real_distribution<double> range(0.0, 1.0);

    for (const auto &conf : vconf) {
        switch (conf.operation) {
            case LogSample: {
                std::cout << "LogSample: shuffling. Keep Low = " << conf.keep_low_up_otherwise << std::endl;
                std::shuffle(log.begin(), log.end(), g);
                auto first = log.begin() + (conf.keep_low_up_otherwise ? 0 : ((size_t) conf.factor * log.size()));
                auto last = log.begin() +
                            (conf.keep_low_up_otherwise ? ((size_t) conf.factor * log.size()) : (log.size() - 1));
                std::vector<Transaction<std::string>> newVec(first, last);
                std::cout << "Reduced size, from " << log.size() << " to " << newVec.size() << std::endl;
                log = newVec;
            }
                break;

            case LogFilterFrequency: {
                std::vector<Transaction<std::string>> output;
                std::map<double, std::vector<size_t>> ordering;
                double N = log.size();
                std::cout << "LogSample: histogramming. Keep Low = " << conf.keep_low_up_otherwise << std::endl;
                for (size_t i = 0, n = log.size(); i < n; i++) {
                    ordering[log[i].size()].emplace_back(i);
                }
                if (conf.keep_low_up_otherwise) {
                    for (auto it = ordering.begin(), en = ordering.end(); it != en; it++) {
                        for (const auto &i : it->second) {
                            output.emplace_back(log[i]);
                        }
                        if (conf.factor <= ((double) ordering.size()) / N) break;
                    }
                } else {
                    for (auto it = ordering.rbegin(), en = ordering.rend(); it != en; it++) {
                        for (const auto &i : it->second) {
                            output.emplace_back(log[i]);
                        }
                        if (conf.factor <= ((double) ordering.size()) / N) break;
                    }
                }
                std::cout << "Reduced size, from " << log.size() << " to " << output.size() << std::endl;
                log = output;
            }
                break;

            case LogFilterValues: {
                double max = -1.0;
                double min = std::numeric_limits<double>::max();
                for (const auto &t : log) {
                    double x = t.size();
                    max = std::max(max, x);
                    min = std::min(min, x);
                }
                double value = conf.factor * (max - min) + min;
                std::cout << "Log max = " << max << " Log min = " << min << " LogFilterValue = " << conf.factor
                          << " Limit value = " << value << " Keep Low = " << conf.keep_low_up_otherwise << std::endl;
                std::vector<Transaction<std::string>> output;
                for (const auto &t : log) {
                    if (conf.keep_low_up_otherwise) {
                        if (((double) t.size()) <= value) output.emplace_back(t);
                    } else {
                        if (((double) t.size()) >= value) output.emplace_back(t);
                    }
                }
                std::cout << "Reduced size, from " << log.size() << " to " << output.size() << std::endl;
                log = output;
            }
                break;
        }
    }


}
