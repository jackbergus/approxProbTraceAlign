//
// Created by giacomo on 11/11/20.
//

#include "utils/SimpleTraceFromFile.h"
#include <sstream>
#include <fstream>

Transaction<std::string> transaction_from_string(const std::string &transaction_line, const char sep) {
    std::stringstream test(transaction_line);
    std::string segment;
    Transaction<std::string> seglist;
    while(std::getline(test, segment, sep))
    {
        seglist.push_back(segment);
    }
    return seglist;
}

std::vector<Transaction<std::string>> read_log(const std::string &file_path, const char sep) {

    std::ifstream infile(file_path);
    std::string line;
    std::vector<Transaction<std::string>> log;
    while (std::getline(infile, line)) {
        log.emplace_back(transaction_from_string(line, sep));
    }
    return log;

}



double spd_we::q_I(const std::vector<Transaction<std::string>> &L, const std::string &label) {
    double count = 0.0;
    for (const auto& x : L) {
        if ((!x.empty()) && (*x.begin() == label)) count++;
    }
    return count;
}

double spd_we::q_F(const std::vector<Transaction<std::string>> &L, const std::string &label) {
    double count = 0.0;
    for (const auto& x : L) {
        if ((!x.empty()) && (*x.rbegin() == label)) count++;
    }
    return count;
}

#include <functional>
#include <numeric>

double spd_we::q_P(const std::vector<Transaction<std::string>> &L, const std::string &s, const std::string &t) {
    std::vector<std::string> varsigma{s, t};
    return std::accumulate(L.begin(), L.end(), 0.0, [&varsigma](double prev, const Transaction<std::string>& t) { return prev + countFreq(varsigma, t); });
}

double spd_we::w_freq(const std::vector<Transaction<std::string>> &L, const std::string &t_label) {
    std::vector<std::string> varsigma{t_label};
    return std::max(1.0, std::accumulate(L.begin(), L.end(), 0.0, [&varsigma](double prev, const Transaction<std::string>& t) { return prev + countFreq(varsigma, t);} ));
}
