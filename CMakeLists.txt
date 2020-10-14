cmake_minimum_required(VERSION 3.16)
project(fuzzyStringMatching2)

set(CMAKE_CXX_STANDARD 20)


find_package (Eigen3 3.3 REQUIRED NO_MODULE)
include_directories(include)
add_executable(fuzzyStringMatching2 main.cpp src/ReadGraph.cpp include/ReadGraph.h src/Iterator.cpp include/Iterator.h)
target_link_libraries(fuzzyStringMatching2 Eigen3::Eigen)