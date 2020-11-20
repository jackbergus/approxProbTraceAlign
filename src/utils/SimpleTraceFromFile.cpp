//
// Created by giacomo on 11/11/20.
//

#include "data_loading/SimpleTraceFromFile.h"
#include <sstream>
#include <fstream>
#include <data_loading/local_load_data.h>

Transaction<TimestampedEvent> transaction_from_string(const std::string &transaction_line, const char sep) {
    std::stringstream test(transaction_line);
    std::string segment;
    long long int current = 0;
    Transaction<TimestampedEvent> seglist;
    while(std::getline(test, segment, sep))
    {
        seglist.emplace_back(segment, current++);
    }
    return seglist;
}

std::vector<Transaction<TimestampedEvent>> read_log(const std::string &file_path, const char sep) {

    std::ifstream infile(file_path);
    std::string line;
    std::vector<Transaction<TimestampedEvent>> log;
    while (std::getline(infile, line)) {
        log.emplace_back(transaction_from_string(line, sep));
    }
    return log;

}

size_t countFreq2(const Transaction<std::string> &pat, const Transaction<TimestampedEvent> &txt) {
    size_t M = pat.size();
    size_t N = txt.size();
    size_t res = 0;

    if (pat.empty()) return txt.size();

    /* A loop to slide pat[] one by one */
    for (int i = 0; i <= N - M; i++)
    {
        /* For current index i, check for
           pattern match */
        int j;
        for (j = 0; j < M; j++)
            if (txt[i+j].event_name != pat[j])
                break;

        // if pat[0...M-1] = txt[i, i+1, ...i+M-1]
        if (j == M)
        {
            res++;
            j = 0;
        }
    }
    return res;
}


double spd_we::q_I(const std::vector<Transaction<TimestampedEvent>> &L, const std::string &label, const std::string& varepsilon) {
    double count = 0.0;
    if (label == varepsilon) return (double)L.size();
    for (const auto& x : L) {
        if ((!x.empty()) && (x.begin()->event_name == label)) count++;
    }
    return count;
}

double spd_we::q_F(const std::vector<Transaction<TimestampedEvent>> &L, const std::string &label, const std::string& varepsilon) {
    double count = 0.0;
    if (label == varepsilon) return (double)L.size();
    for (const auto& x : L) {
        if ((!x.empty()) && (x.rbegin()->event_name == label)) count++;
    }
    return count;
}

#include <functional>
#include <numeric>



double spd_we::q_P(const std::vector<Transaction<TimestampedEvent>> &L, const std::string &s, const std::string &t, const std::string& varepsilon) {
    std::vector<std::string> varsigma;
    if (s != varepsilon) varsigma.emplace_back(s);
    if (t != varepsilon) varsigma.emplace_back(t);
    return std::accumulate(L.begin(), L.end(), 0.0, [&varsigma](double prev, const Transaction<TimestampedEvent>& t) { return prev + countFreq2(varsigma, t); });
}

double spd_we::w_freq(const std::vector<Transaction<TimestampedEvent>> &L, const std::string &t_label, const std::string& varepsilon) {
    std::vector<std::string> varsigma;
    if (t_label != varepsilon) varsigma.emplace_back(t_label);
    return std::max(1.0, std::accumulate(L.begin(), L.end(), 0.0, [&varsigma](double prev, const Transaction<TimestampedEvent>& t) { return prev + countFreq2(varsigma, t);} ));
}
