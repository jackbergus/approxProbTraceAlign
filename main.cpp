

#include <topk/Aussageform/DistanceExpressionEvaluator.h>

// Default main that is run at production
int official_main(int argc, char* argv[]);


#include <unicode/datefmt.h>
#include <unicode/smpdtfmt.h>

//#define DEBUG
#include <chrono>
#include <iomanip>
#include <date.h>



void sandbox() {
    /*long long int milliSecondsSinceEpoch =
    std::chrono::duration_cast<std::chrono::milliseconds>(parse8601("2014-11-11T14:26:44.000Z").time_since_epoch()).count();
    const auto durationSinceEpoch = std::chrono::milliseconds(milliSecondsSinceEpoch);
    const std::chrono::time_point<std::chrono::system_clock> tp_after_duration(durationSinceEpoch);
    time_t time_after_duration = std::chrono::system_clock::to_time_t(tp_after_duration);

    std::tm* formattedTime = std::localtime(&time_after_duration);

    long long int milliseconds_remainder = milliSecondsSinceEpoch % 1000;
    std::cout <<std::put_time(std::localtime(&time_after_duration), "%y-%m-%d-%H-%M-%S-") << milliseconds_remainder << std::endl;*/
    /*DistanceExpressionEvaluator expr{"zip \"x+y\" \"(x-y)*(x-y)\" 0.0 v0 v1"};
    std::cout << expr({1,2,3},{4,5,6}) << std::endl;
    std::cout << expr({1,2,3},{7,5,6}) << std::endl;
    std::cout << expr({1,4,3},{4,5,6}) << std::endl;
    std::cout << expr({1,2,3},{4,5,8}) << std::endl;*/
}

int main(int argc, char* argv[]) {
    #ifdef DEBUG
        // Leaving out some sandbox space
        sandbox();
    #else
        return official_main(argc, argv);

    #endif
}

#include "ConfigurationFile.h"
#include <utils/xml_utils.h>
#include <QtWidgets/QApplication>
#include <gui/WSettings.h>
#include <args.hxx>
#include <sys/wait.h>
#include <unistd.h>


int official_main(int argc, char* argv[]) {

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