cmake_minimum_required(VERSION 3.17)
project(Plotter-SingleScatterPlot LANGUAGES CXX)

include("../../cmake/utils.cmake")
string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

if(is_top_level)
    find_package(Plotter REQUIRED)
endif()

set(sources main.cpp)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})

add_executable(Plotter-SingleScatterPlot)
target_sources(Plotter-SingleScatterPlot PRIVATE ${sources})
target_link_libraries(Plotter-SingleScatterPlot PUBLIC glfw GL dl PRIVATE Plotter::Plotter)

if(NOT is_top_level)
    win_copy_deps_to_target_dir(Plotter-SingleScatterPlot Plotter::Plotter)
endif()