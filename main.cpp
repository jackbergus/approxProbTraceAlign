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
#include "petrinet/PetriNet.h"
#include <utils/xml_utils.h>
#include <QtWidgets/QApplication>
#include <gui/WSettings.h>
#include <args.hxx>
#include <sys/wait.h>
#include <unistd.h>
#include <regex>
#include <fstream>

int main(int argc, char* argv[]) {

#if 0

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
#else

    /*PetriNet pn;
    pn.places = {0,1,2,3,4,5,6};
    pn.transitions = {7,8,9,10,11,12};
    pn.transition_labelling[7] = "t1";
    pn.transition_labelling[8] = "t2";
    pn.transition_labelling[9] = "t3";
    pn.transition_labelling[10] = "t4";
    pn.transition_labelling[11] = "t5";
    pn.transition_labelling[12] = "t6";
    pn.firing_weight[7] = 1.0;
    pn.firing_weight[8] = 1.0;
    pn.firing_weight[9] = 1.0;
    pn.firing_weight[10] = 1.0;
    pn.firing_weight[11] = 1.0;
    pn.firing_weight[12] = 1.0;
    PetriNet::Marking m0 = {1,0,0,0,0,0,0};
    assert(pn.places.size() == m0.size());
    pn.w_set_cost(0, 7,1);

    pn.w_set_cost(1, 8,1);
    pn.w_set_cost(1, 11,1);

    pn.w_set_cost(2, 10,1);

    pn.w_set_cost(3, 12,1);

    pn.w_set_cost(4, 9,1);

    pn.w_set_cost(5, 12,1);

    pn.w_set_cost(7, 1,1);
    pn.w_set_cost(7, 4,1);

    pn.w_set_cost(8, 2,1);

    pn.w_set_cost(9, 5,1);

    pn.w_set_cost(10, 3,1);

    pn.w_set_cost(11, 3,1);

    pn.w_set_cost(12, 6,1);

    pn.generateOutgoingForReachabilityGraph({0, 1, 0, 0, 1, 0, 0});

    // aFTER BUilding the petri net up, setting up the Reachability of it
    const auto rg = pn.generateReachabilityGraph(m0);


    PetriNet::MetaReachabilityGraph mrg;
    size_t eSize_plusOne = 1;
    for (const auto& cp : rg.outgoingEdges) {
        for (const auto& e : cp.second) {
            mrg.node_id_assoc.put(eSize_plusOne++, std::make_pair(cp.first, std::make_pair(e.second, e.first)));
            eSize_plusOne++;
        }
    }
    for (const auto& cp : rg.outgoingEdges) {
        for (const auto& e : cp.second) {
            const auto it = rg.outgoingEdges.find(e.first);
            if (it != rg.outgoingEdges.end()) {
                for (const auto& e2 : it->second) {
                    mrg.outgoingEdges[mrg.node_id_assoc.getKey(std::make_pair(cp.first, std::make_pair(e.second, e.first)))]
                                     [mrg.node_id_assoc.getKey(std::make_pair(e.first, std::make_pair(e2.second, e2.first)))] = e.first;
                }
            }
        }
    }
    if (doAddIState(rg, m0)) {
        mrg.initialEState = {0};
        for (const auto& e : rg.outgoingEdges.at(m0)) {
            mrg.outgoingEdges[0][mrg.node_id_assoc.getKey(std::make_pair(m0, std::make_pair(e.second, e.first)))] = e.first;
        }
    }
    const auto fs = getFinalState(rg);
    if (doAddFState(rg, fs)) {
        mrg.finalEState = {eSize_plusOne};
        for (const auto& out : rg.outgoingEdges) {
            const auto it = out.second.find(fs);
            if (it != out.second.end()) {
                mrg.outgoingEdges[mrg.node_id_assoc.getKey(std::make_pair(out.first, std::make_pair(it->second, it->first)))][eSize_plusOne] = it->first;
            }
        }
    }

    */
    ///load_pnml("/home/giacomo/Immagini/split-miner-2.0/sepsis_23000000.pnml");


    auto graph = load_pnml("/home/giacomo/Immagini/split-miner-2.0/sepsis_23000000.pnml")[0];
    ///std::cerr << "Weight transfer" << std::endl;
    ///graph.transfer_weight_from_nodes_to_edges();
    std::cerr << "Closure" << std::endl;
    //graph.doClosure(".");
    ReadGraph    finalGraph;
    std::cerr << "Loading into the final format" << std::endl;
    finalGraph.init(graph.nodes()+1, graph.countEdges(), graph.getStart(), graph.getEnd());
    finalGraph.name = graph.getName();
    std::cerr << " 1) Loading the vertices and outgoing edges" << std::endl;
    for (const auto& n : graph.getNodes()) {
        std::string s = graph.getNodeLabel(n);
        std::replace( s.begin(), s.end(), ' ', '_');
        finalGraph.addNode(n, s);
        for (const auto& e : graph.outgoing(n)) {
            finalGraph.addEdge(n, e.first, e.second);
        }
    }
    std::cerr << " 2) Finalizing the representation" << std::endl;
    finalGraph.finalizeEdgesMatrix(graph.getCost());
    std::cerr << "Writing at last!" << std::endl;
    std::ofstream save_preprocessing_step{"sepsis_23000000.txt"};
    finalGraph.printStream(save_preprocessing_step);

#endif
}
