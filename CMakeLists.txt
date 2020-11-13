cmake_minimum_required(VERSION 3.0.0)
project(fuzzyStringMatching2)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(submodules/yaml)
include_directories(submodules/yaml/include)

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
        antlr4/cpp/antlr4/regexVisitor.cpp src/RegexVisitor.cpp include/RegexVisitor.h )

set(qtgui src/gui/WSettings.cpp src/gui/QTUtils.cpp include/gui/QTUtils.h src/gui/QTBuilder.cpp include/gui/QTBuilder.h QTSpinBoxDelegate.cpp QTDropdownDelegate.cpp QTDropdownDelegate.h)

add_library(rapidxml INTERFACE)
target_include_directories(rapidxml INTERFACE rapidxml/ )

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

add_subdirectory(submodules/gvpp)
include_directories(submodules/gvpp)


set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ ${CMAKE_MODULE_PATH})
#add_subdirectory(submodules/graphviz)
#include_directories(submodules/graphviz/lib)
#include_directories(submodules/graphviz/lib/common)
#include_directories(submodules/graphviz/lib/gvc)
#include_directories(submodules/graphviz/lib/pathplan)
#include_directories(submodules/graphviz/lib/cgraph)
#include_directories(submodules/graphviz/lib/cdt)

add_subdirectory(submodules/args)
include_directories(submodules/args)

option(ENABLE_RPATH "Enable RPATH for this library" ON)
mark_as_advanced(ENABLE_RPATH)
find_package(Graphviz)
include_directories(${GRAPHVIZ_INCLUDE_DIR})
find_package(faiss)
find_package(OpenMP)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()
find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(fuzzyStringMatching2 ${antlr_sources} ${qtgui} main.cpp src/distances/strings/LevensteinDistance.cpp src/benchmarking/Ranking.cpp include/embeddings/path_embedding/EmbedPahtsOverSingleGraphStrategy.h src/matrix_graph_path/PathIterator.cpp include/matrix_graph_path/PathIterator.h src/matrix_graph_path/ConditionalReadGraphIterable.cpp include/matrix_graph_path/ConditionalReadGraphIterable.h src/ReadGraph.cpp include/ReadGraph.h src/embeddings/labelled_paths/OnlyTransitiveEdgesCost.cpp include/embeddings/labelled_paths/OnlyTransitiveEdgesCost.h include/Regex.h src/matrix_graph_path/stack_info.cpp include/matrix_graph_path/stack_info.h src/matrix_graph_path/path_info.cpp include/matrix_graph_path/path_info.h src/embeddings/labelled_paths/LabelledPathVisitingStrategy.cpp include/embeddings/labelled_paths/LabelledPathVisitingStrategy.h src/embeddings/path_embedding/MultiplePathsEmbeddingStrategy.cpp include/embeddings/path_embedding/MultiplePathsEmbeddingStrategy.h src/benchmarking/rows/TimeBenchmarkRowEntry.cpp src/utils/hash_combine.cpp src/benchmarking/BenchmarkStrategy.cpp include/distances/path/GenericStringCostFunction.h src/benchmarking/rows/PrecisionBenchmarkRowEntry.cpp include/benchmarking/rows/PrecisionBenchmarkRowEntry.h src/benchmarking/RankPathCollectionPathInfo.cpp include/benchmarking/RankPathCollectionPathInfo.h src/distances/strings/LevensteinSimilarity.cpp include/distances/strings/LevensteinSimilarity.h src/benchmarking/ReadGraphRankingStruct.cpp include/benchmarking/ReadGraphRankingStruct.h src/points/PointClass.cpp include/points/PointClass.h src/points/PointClassVector.cpp include/points/PointClassVector.h src/points/PointTransformation.cpp include/points/PointTransformation.h RankingTest.cpp RankingTest.h src/utils/SimpleTraceFromFile.cpp include/utils/SimpleTraceFromFile.h src/ConfigurationFile.cpp src/utils/xml_utils.cpp src/ThomsonNFA.cpp include/ThomsonNFA.h src/InputStream.cpp include/InputStream.h src/utils/AlterString.cpp include/utils/AlterString.h include/utils/pair_hash.h src/embeddings/labelled_paths/NodesWithTransitiveEdgeCost.cpp include/embeddings/labelled_paths/NodesWithTransitiveEdgeCost.h src/embeddings/graph_embedding/GraphEmbeddingStrategy.cpp include/embeddings/graph_embedding/GraphEmbeddingStrategy.h src/benchmarking/RankPathCollectionReadGraph.cpp include/benchmarking/RankPathCollectionReadGraph.h src/distances/path/LevensteinPathSimilarity.cpp include/distances/path/LevensteinPathSimilarity.h include/RankPathCollectionParameter.h src/benchmarking/BenchmarkConfiguration.cpp include/benchmarking/BenchmarkConfiguration.h GraphCartridge.cpp GraphCartridge.h include/GenericGraph.h include/utils/fixed_bimap.h)
target_link_libraries(fuzzyStringMatching2 yaml-cpp antlr4_static Eigen3::Eigen fort OpenMP::OpenMP_CXX faiss gvpp
        ${GRAPHVIZ_cdt_LIBRARY} ${GRAPHVIZ_graph_LIBRARY} ${GRAPHVIZ_gvc_LIBRARY} ${GRAPHVIZ_pathplan_LIBRARY} Qt5::Core Qt5::Widgets)