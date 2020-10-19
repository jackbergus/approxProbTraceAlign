cmake_minimum_required(VERSION 3.16)
project(fuzzyStringMatching2)

set(CMAKE_CXX_STANDARD 20)


add_subdirectory(submodules/antlr4/runtime/Cpp)
include_directories(submodules/antlr4/runtime/Cpp/runtime/src)
include_directories(antlr4/cpp/antlr4)

set(antlr_sources antlr4/cpp/antlr4/regexBaseListener.cpp
        antlr4/cpp/antlr4/regexBaseVisitor.cpp
        antlr4/cpp/antlr4/regexLexer.cpp
        antlr4/cpp/antlr4/regexListener.cpp
        antlr4/cpp/antlr4/regexParser.cpp
        antlr4/cpp/antlr4/regexVisitor.cpp src/RegexVisitor.cpp include/RegexVisitor.h src/ThomsonNFA.cpp include/ThomsonNFA.h src/InputStream.cpp include/InputStream.h src/AlterString.cpp include/AlterString.h include/utils/pair_hash.h src/embeddings/labelled_paths/NodesWithTransitiveEdgeCost.cpp include/embeddings/labelled_paths/NodesWithTransitiveEdgeCost.h src/embeddings/graph_embedding/GraphEmbeddingStrategy.cpp include/embeddings/graph_embedding/GraphEmbeddingStrategy.h)

add_subdirectory(submodules/lemon)
include_directories(submodules/lemon)
include_directories(cmake-build-debug/submodules/lemon)
include_directories(submodules/generator)

#find_package(antlr4-generator REQUIRED)
#find_package(antlr4-runtime REQUIRED)

find_package (Eigen3 3.3 REQUIRED NO_MODULE)
find_package(Boost 1.71 REQUIRED)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
include_directories(include)
include_directories(submodules/json/include)
add_executable(fuzzyStringMatching2 ${antlr_sources} main.cpp include/embeddings/path_embedding/EmbedPahtsOverSingleGraphStrategy.h src/matrix_graph_path/PathIterator.cpp include/matrix_graph_path/PathIterator.h src/matrix_graph_path/ConditionalReadGraphIterable.cpp include/matrix_graph_path/ConditionalReadGraphIterable.h src/ReadGraph.cpp include/ReadGraph.h src/embeddings/labelled_paths/OnlyTransitiveEdgesCost.cpp include/embeddings/labelled_paths/OnlyTransitiveEdgesCost.h include/Regex.h src/matrix_graph_path/stack_info.cpp include/matrix_graph_path/stack_info.h src/matrix_graph_path/path_info.cpp include/matrix_graph_path/path_info.h src/embeddings/labelled_paths/LabelledPathVisitingStrategy.cpp include/embeddings/labelled_paths/LabelledPathVisitingStrategy.h src/embeddings/path_embedding/MultiplePathsEmbeddingStrategy.cpp include/embeddings/path_embedding/MultiplePathsEmbeddingStrategy.h)
target_link_libraries(fuzzyStringMatching2 antlr4_static Eigen3::Eigen)