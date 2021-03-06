/*
 * main.cpp
 * This file is part of ProbabilisticTraceAlignment
 *
 * Copyright (C) 2020 - Giacomo Bergami
 *
 * ProbabilisticTraceAlignment is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * ProbabilisticTraceAlignment is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ProbabilisticTraceAlignment. If not, see <http://www.gnu.org/licenses/>.
 */

 

#include <iomanip>
#include "ConfigurationFile.h"
#include <utils/xml_utils.h>
#include <QtWidgets/QApplication>
#include <gui/WSettings.h>
#include <args.hxx>
#include <sys/wait.h>
#include <unistd.h>


int main(int argc, char* argv[]) {

    args::ArgumentParser parser("FuzzyStringMatching (2) (c) 2020-2021 by Giacomo Bergami.", "This free and open software program implements the (Approximate) Probabilistic Trace Alignment. Youse at your own risk.");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
    args::Group group(parser, "You can use the following parameters", args::Group::Validators::DontCare, args::Options::Global);
    args::Flag gui(group, "gui", "Shows the GUI for setting the specified configuration file (when no parameter is specified, this is the default behaviour). The gui will store the resulting configuration file file once the window is closed.", {'g', "gui"});
    args::Flag run(group, "run", "Runs the program accordingly to the configuration file. If both --gui and --run are set, first the GUI is displayed, and then the benchmark is run", {'r', "run"});
    args::ValueFlag<std::string> con(group, "configuration.yaml", "Specifies the configuration file to edit (--gui) or to use to run the program (--run). If no configuration file is specified, the program will look for 'configuration.yaml'. If that file is not provided, a default configuration will be set-up. Run --gui to see the file", {'c', "conf"});
    try {
        parser.ParseCLI(argc, argv);
    } catch (args::Help& ) {
        std::cout << parser;
        return 0;
    } catch (args::ParseError& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    } catch (args::ValidationError& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }

    std::string yaml = "configuration.yaml";
    if (con) {
        yaml = args::get(con);
    }

    if (gui && run) { // Running the gui, and then possibly run the benchmarks
        pid_t child = fork();
        if (child) {
            wait(nullptr); // Unfortunately, by quitting the window, the process will terminate. Therefore, I fork a child process for the gui, I wait for it, and then I run the benchmarks
        } else {
            QApplication app(argc, (char**)argv);
            WSettings window{yaml};
            window.show();
            return app.exec();
        }
    }

    if (run) {
        ConfigurationFile conf{yaml};
        conf.run();
    } else if (gui) {
        QApplication app(argc, (char**)argv);
        WSettings window{yaml};
        window.show();
        return app.exec();
    } else {
        std::cout << parser;
    }
    return 0;
}
