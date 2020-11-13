//
// Created by giacomo on 13/11/20.
//

#include <data_loading/load_matrix.h>
#include <utils/cfiles.h>

GenericGraph<size_t> load_matrix(const std::string &filename) {
    GenericGraph<size_t> graph;
    FILE* file = fopen(filename.c_str(), "r");
    size_t error = 0;
    size_t nodes = 0, edges = 0;
    size_t source = 0, target = 0;
    if (file) {
        int i;
        double w = 1;
        // Reading the number of the nodes
        i = fscanf(file, "nodes: %zd\n", &nodes);
        error = (i == EOF || (i != 1));
        if (error) return graph;
        i = fscanf(file, "edges: %zd\n", &edges);
        error = (i == EOF || (i != 1));
        if (error) return graph;
        i = fscanf(file, "source: %zd\n", &source);
        error = (i == EOF || (i != 1));
        if (error) return graph;
        graph.setStart(source);
        i = fscanf(file, "target: %zd\n", &target);
        error = (i == EOF || (i != 1));
        if (error) return graph;
        graph.setEnd(target);
        i = fscanf(file, "weight: %lf\n", &w);
        error = (i == EOF || (i != 1));
        if (error) return graph;
        graph.setWeight(w);

        for (size_t j = 0; j<nodes; j++) {
            size_t node_no;
            char string[124];
            std::string k;
            i = fscanf(file, "%zd %123s\n", &node_no, string);
            error = (i == EOF || (i != 2));
            if (error) return graph;
            k = std::string(string);
            graph.add_node(node_no, k);
        }


        for (size_t j = 0; j<edges; j++) {
            size_t src, dst;
            double weight;
            i = fscanf(file, "%zd %zd %lf\n", &src, &dst, &weight);
            error = (i == EOF || (i != 3));
            if (error) return graph;
            assert(std::abs(weight) <= 1.0);
            graph.add_edge(src, dst, weight);
        }
        fclose(file);
    }
    return graph;
}
