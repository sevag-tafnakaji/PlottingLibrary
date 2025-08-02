cmake_minimum_required(VERSION 3.17)
project(Plotter-SingleLinePlot LANGUAGES CXX)

include("../../cmake/utils.cmake")
string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

if(is_top_level)
    find_package(Plotter REQUIRED)
endif()

set(sources main.cpp)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})

add_executable(Plotter-SingleLinePlot)
target_sources(Plotter-SingleLinePlot PRIVATE ${sources})
target_link_libraries(Plotter-SingleLinePlot PUBLIC glfw GL dl PRIVATE Plotter::Plotter)

if(NOT is_top_level)
    win_copy_deps_to_target_dir(Plotter-SingleLinePlot Plotter::Plotter)
endif()