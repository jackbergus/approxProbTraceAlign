cmake_minimum_required(VERSION 3.16)
project(fuzzyStringMatching2)

set(CMAKE_CXX_STANDARD 20)

include_directories(submodules/maigc_enum/include)

set(FORT_ENABLE_TESTING OFF CACHE INTERNAL "")
add_subdirectory(submodules/libfort)
include_directories(submodules/libfort/lib)

add_subdirectory(submodules/antlr4/runtime/Cpp)
include_directories(submodules/antlr4/runtime/Cpp/runtime/src)
include_directories(antlr4/cpp/antlr4)

set(antlr_sources antlr4/cpp/antlr4/regexBaseListener.cpp
        antlr4/cpp/antlr4/regexBaseVisitor.cpp
        antlr4/cpp/antlr4/regexLexer.cpp
        antlr4/cpp/antlr4/regexListener.cpp
        antlr4/cpp/antlr4/regexParser.cpp
        antlr4/cpp/antlr4/regexVisitor.cpp src/RegexVisitor.cpp include/RegexVisitor.h src/ThomsonNFA.cpp include/ThomsonNFA.h src/InputStream.cpp include/InputStream.h src/utils/AlterString.cpp include/utils/AlterString.h include/utils/pair_hash.h src/embeddings/labelled_paths/NodesWithTransitiveEdgeCost.cpp include/embeddings/labelled_paths/NodesWithTransitiveEdgeCost.h src/embeddings/graph_embedding/GraphEmbeddingStrategy.cpp include/embeddings/graph_embedding/GraphEmbeddingStrategy.h src/benchmarking/RankPathCollectionReadGraph.cpp include/benchmarking/RankPathCollectionReadGraph.h src/distances/path/LevensteinPathSimilarity.cpp include/distances/path/LevensteinPathSimilarity.h include/RankPathCollectionParameter.h src/benchmarking/BenchmarkConfiguration.cpp include/benchmarking/BenchmarkConfiguration.h GraphCartridge.cpp GraphCartridge.h)

add_subdirectory(submodules/lemon)
include_directories(submodules/lemon)
include_directories(cmake-build-debug/submodules/lemon)
include_directories(submodules/generator)

include_directories(submodules/fpgrowth/include)

#find_package(antlr4-generator REQUIRED)
#find_package(antlr4-runtime REQUIRED)

find_package (Eigen3 3.3 REQUIRED NO_MODULE)
find_package(Boost 1.71 REQUIRED)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
include_directories(include)
include_directories(submodules/json/include)

find_package(faiss)
find_package(OpenMP)


add_executable(fuzzyStringMatching2 ${antlr_sources} main.cpp src/distances/strings/LevensteinDistance.cpp src/benchmarking/Ranking.cpp include/embeddings/path_embedding/EmbedPahtsOverSingleGraphStrategy.h src/matrix_graph_path/PathIterator.cpp include/matrix_graph_path/PathIterator.h src/matrix_graph_path/ConditionalReadGraphIterable.cpp include/matrix_graph_path/ConditionalReadGraphIterable.h src/ReadGraph.cpp include/ReadGraph.h src/embeddings/labelled_paths/OnlyTransitiveEdgesCost.cpp include/embeddings/labelled_paths/OnlyTransitiveEdgesCost.h include/Regex.h src/matrix_graph_path/stack_info.cpp include/matrix_graph_path/stack_info.h src/matrix_graph_path/path_info.cpp include/matrix_graph_path/path_info.h src/embeddings/labelled_paths/LabelledPathVisitingStrategy.cpp include/embeddings/labelled_paths/LabelledPathVisitingStrategy.h src/embeddings/path_embedding/MultiplePathsEmbeddingStrategy.cpp include/embeddings/path_embedding/MultiplePathsEmbeddingStrategy.h src/benchmarking/rows/TimeBenchmarkRowEntry.cpp src/utils/hash_combine.cpp src/benchmarking/BenchmarkStrategy.cpp include/distances/path/GenericStringCostFunction.h src/benchmarking/rows/PrecisionBenchmarkRowEntry.cpp include/benchmarking/rows/PrecisionBenchmarkRowEntry.h src/benchmarking/RankPathCollectionPathInfo.cpp include/benchmarking/RankPathCollectionPathInfo.h src/distances/strings/LevensteinSimilarity.cpp include/distances/strings/LevensteinSimilarity.h src/benchmarking/ReadGraphRankingStruct.cpp include/benchmarking/ReadGraphRankingStruct.h src/points/PointClass.cpp include/points/PointClass.h src/points/PointClassVector.cpp include/points/PointClassVector.h src/points/PointTransformation.cpp include/points/PointTransformation.h RankingTest.cpp RankingTest.h SimpleTraceFromFile.cpp SimpleTraceFromFile.h StochasticPetriNet.cpp StochasticPetriNet.h)
target_link_libraries(fuzzyStringMatching2 antlr4_static Eigen3::Eigen fort OpenMP::OpenMP_CXX faiss)