//
// Created by giacomo on 28/11/20.
//

#include "utils/ProgressBar.h"

void progress_bar::write(double fraction)
{
    // clamp fraction to valid range [0,1]
    if (fraction < 0)
        fraction = 0;
    else if (fraction > 1)
        fraction = 1;

    auto width = bar_width - message.size();
    auto offset = bar_width - static_cast<unsigned>(width * fraction);

    os << '\r' << message;
    os.write(full_bar.data() + offset, width);
    os << " [" << std::setw(3) << static_cast<int>(100*fraction) << "%] " << std::flush;
}

progress_bar::~progress_bar() {
    write(1.0);
    os << '\n';
}

progress_bar::progress_bar(std::ostream &os, std::size_t line_width, std::string message_, const char symbol)
        : os{os},
          bar_width{line_width - overhead},
          message{std::move(message_)},
          full_bar{std::string(bar_width, symbol) + std::string(bar_width, ' ')}
{
    if (message.size()+1 >= bar_width || message.find('\n') != message.npos) {
        os << message << '\n';
        message.clear();
    } else {
        message += ' ';
    }
    write(0.0);
}
