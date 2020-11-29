//
// Created by giacomo on 28/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_PROGRESSBAR_H
#define FUZZYSTRINGMATCHING2_PROGRESSBAR_H


#include <cmath>
#include <iomanip>
#include <ostream>
#include <string>

/// https://codereview.stackexchange.com/a/186537/222426
class progress_bar
{
    static const auto overhead = sizeof " [100%]";

    std::ostream& os;
    const std::size_t bar_width;
    std::string message;
    const std::string full_bar;

public:
    progress_bar(std::ostream& os, std::size_t line_width,
                 std::string message_, const char symbol = '.');

    // not copyable
    progress_bar(const progress_bar&) = delete;
    progress_bar& operator=(const progress_bar&) = delete;

    ~progress_bar();

    void write(double fraction);
};



#endif //FUZZYSTRINGMATCHING2_PROGRESSBAR_H
