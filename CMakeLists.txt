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
        antlr4/cpp/antlr4/regexVisitor.cpp src/RegexVisitor.cpp include/RegexVisitor.h src/ThomsonNFA.cpp include/ThomsonNFA.h)

add_subdirectory(submodules/lemon)
include_directories(submodules/lemon)
include_directories(cmake-build-debug/submodules/lemon)

#find_package(antlr4-generator REQUIRED)
#find_package(antlr4-runtime REQUIRED)

find_package (Eigen3 3.3 REQUIRED NO_MODULE)
include_directories(include)
add_executable(fuzzyStringMatching2 ${antlr_sources} main.cpp src/ReadGraph.cpp include/ReadGraph.h src/Iterator.cpp include/Iterator.h include/Regex.h)
target_link_libraries(fuzzyStringMatching2 antlr4_static Eigen3::Eigen)