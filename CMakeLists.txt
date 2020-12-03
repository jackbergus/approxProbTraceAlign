cmake_minimum_required(VERSION 3.0.0)
project(fuzzyStringMatching2)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(submodules/yaml)
include_directories(submodules/yaml/include)

include_directories(submodules/maigc_enum/include)

set(FORT_ENABLE_TESTING OFF CACHE INTERNAL "")
add_subdirectory(submodules/antlr4/runtime/Cpp)
include_directories(submodules/antlr4/runtime/Cpp/runtime/src)
include_directories(antlr4/cpp/antlr4)

set(antlr_sources antlr4/cpp/antlr4/regexBaseListener.cpp
        antlr4/cpp/antlr4/regexBaseVisitor.cpp
        antlr4/cpp/antlr4/regexLexer.cpp
        antlr4/cpp/antlr4/regexListener.cpp
        antlr4/cpp/antlr4/regexParser.cpp
        antlr4/cpp/antlr4/regexVisitor.cpp
        src/data_loading/regex/RegexVisitor.cpp

        antlr4/cpp/antlr4/expressionBaseListener.cpp
        antlr4/cpp/antlr4/expressionBaseVisitor.cpp
        antlr4/cpp/antlr4/expressionLexer.cpp
        antlr4/cpp/antlr4/expressionListener.cpp
        antlr4/cpp/antlr4/expressionParser.cpp
        antlr4/cpp/antlr4/expressionVisitor.cpp
        src/topk/Aussageform/DistanceExpressionEvaluator.cpp

        include/data_loading/regex/RegexVisitor.h)

set(qtgui src/gui/WSettings.cpp src/gui/QTUtils.cpp include/gui/QTUtils.h src/gui/QTBuilder.cpp include/gui/QTBuilder.h QTSpinBoxDelegate.cpp QTDropdownDelegate.cpp QTDropdownDelegate.h include/data_loading/local_load_data.h)

add_library(rapidxml INTERFACE)
target_include_directories(rapidxml INTERFACE rapidxml/ )


include_directories(submodules/generator)
include_directories(submodules/date/include/date)
include_directories(submodules/fpgrowth/include)

#find_package(antlr4-generator REQUIRED)
#find_package(antlr4-runtime REQUIRED)

#find_package (Eigen3 3.3 REQUIRED NO_MODULE)
add_subdirectory(submodules/eigen)
include_directories(submodules/eigen)
find_package(Boost 1.71 REQUIRED)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
include_directories(include)
include_directories(submodules/json/include)
include_directories(submodules/exprtk)

add_subdirectory(submodules/gvpp)
include_directories(submodules/gvpp)


set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ ${CMAKE_MODULE_PATH})

add_subdirectory(submodules/args)
include_directories(submodules/args)

option(ENABLE_RPATH "Enable RPATH for this library" ON)
mark_as_advanced(ENABLE_RPATH)
find_package(Graphviz)
include_directories(${GRAPHVIZ_INCLUDE_DIR})
find_package(OpenMP)

include_directories(submodules/date/include/date)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()
find_package(Qt5 COMPONENTS Widgets REQUIRED)
include_directories(submodules/knn-cpp/include)
add_executable(fuzzyStringMatching2 ${antlr_sources} ${qtgui} ${topk} main.cpp src/utils/ProgressBar.cpp src/petrinet/EClosPhase1.cpp src/petrinet/MetaReachabilityGraph.cpp src/petrinet/ReachabilityGraph.cpp src/petrinet/PetriNet.cpp src/distances/strings/LevensteinDistance.cpp include/embeddings/path_embedding/EmbedPahtsOverSingleGraphStrategy.h src/matrix_graph_path/PathIterator.cpp include/matrix_graph_path/PathIterator.h src/matrix_graph_path/ConditionalReadGraphIterable.cpp include/matrix_graph_path/ConditionalReadGraphIterable.h src/ReadGraph.cpp include/ReadGraph.h include/embeddings/labelled_paths/OnlyTransitiveEdgesCost.h include/data_loading/regex/Regex.h src/matrix_graph_path/stack_info.cpp include/matrix_graph_path/stack_info.h src/matrix_graph_path/path_info.cpp include/matrix_graph_path/path_info.h src/embeddings/labelled_paths/LabelledPathVisitingStrategy.cpp include/embeddings/labelled_paths/LabelledPathVisitingStrategy.h src/embeddings/path_embedding/MultiplePathsEmbeddingStrategy.cpp include/embeddings/path_embedding/MultiplePathsEmbeddingStrategy.h src/utils/hash_combine.cpp include/distances/path/GenericStringCostFunction.h src/distances/strings/LevensteinSimilarity.cpp include/distances/strings/LevensteinSimilarity.h  src/utils/SimpleTraceFromFile.cpp include/data_loading/SimpleTraceFromFile.h src/ConfigurationFile.cpp src/utils/xml_utils.cpp  src/utils/AlterString.cpp include/utils/AlterString.h include/utils/pair_hash.h include/embeddings/labelled_paths/NodesWithTransitiveEdgeCost.h include/embeddings/graph_embedding/GraphEmbeddingStrategy.h src/distances/path/LevensteinPathSimilarity.cpp include/distances/path/LevensteinPathSimilarity.h include/RankPathCollectionParameter.h include/GenericGraph.h include/utils/fixed_bimap.h src/data_loading/load_xes.cpp include/data_loading/load_xes.h src/data_loading/load_pnml.cpp include/data_loading/load_pnml.h src/data_loading/load_bpmn.cpp include/data_loading/load_bpmn.h src/data_loading/load_petri_matrix.cpp include/data_loading/load_petri_matrix.h src/data_loading/local_load_data.cpp src/data_loading/load_matrix.cpp include/data_loading/load_matrix.h src/data_loading/load_pregex.cpp include/data_loading/load_pregex.h src/log/LogOperationConfiguration.cpp include/log/LogOperationConfiguration.h src/log/log_operations.cpp include/log/log_operations.h src/topk/Aussageform/DistanceExpressionEvaluator.cpp include/topk/Aussageform/DistanceExpressionEvaluator.h src/topk/Aussageform/binary_expression_compiler.cpp include/topk/Aussageform/binary_expression_compiler.h src/benchmarking/ScoringUtils.cpp src/benchmarking/minimum_edit_maximum_substring.cpp src/topk/algorithm_map.cpp)
target_link_libraries(fuzzyStringMatching2 yaml-cpp antlr4_static  OpenMP::OpenMP_CXX gvpp
        ${GRAPHVIZ_cdt_LIBRARY} ${GRAPHVIZ_graph_LIBRARY} ${GRAPHVIZ_gvc_LIBRARY} ${GRAPHVIZ_pathplan_LIBRARY} Qt5::Core Qt5::Widgets)
