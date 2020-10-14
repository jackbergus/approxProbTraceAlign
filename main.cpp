


#include "ReadGraph.h"

int main() {
    ReadGraph t{"matrix.txt"};
    ///t.generateDecomposedEmbeddings(1.0);
    ///t.printAllPathsEmbeddings();

    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;

    t.decomposedEmbedding.clear();
    t.decomposeStart(1.0);
    t.printAllPathsEmbeddings();
}
