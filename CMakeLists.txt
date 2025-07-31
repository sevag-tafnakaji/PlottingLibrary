cmake_minimum_required(VERSION 3.17)
project(Plotter VERSION 0.0.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#----------------------------------------------------------------------------------------------------------------------
# general settings and options
#----------------------------------------------------------------------------------------------------------------------

include(cmake/utils.cmake)
include(GNUInstallDirs)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

# PLOTTER_SHARED_LIBS option (undefined by default) can be used to force shared/static build
option(PLOTTER_BUILD_TESTS "Build plotter tests" OFF)
option(PLOTTER_BUILD_EXAMPLES "Build plotter examples" ON)
option(PLOTTER_BUILD_DOCS "Build plotter documentation" OFF)
# option(PLOTTER_INSTALL "Generate target for installing plotter" ${is_top_level})
option(PLOTTER_INSTALL "Generate target for installing plotter" ON)
set_if_undefined(PLOTTER_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/plotter" CACHE STRING
    "Install path for plotter package-related CMake files")

if(DEFINED PLOTTER_SHARED_LIBS)
    set(BUILD_SHARED_LIBS ${PLOTTER_SHARED_LIBS})
endif()

if(NOT DEFINED CMAKE_BUILD_TYPE AND NOT DEFINED CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set_if_undefined(CMAKE_CXX_VISIBILITY_PRESET hidden)
set_if_undefined(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

add_library(Plotter) # initialized below
add_library(Plotter::Plotter ALIAS Plotter)

#----------------------------------------------------------------------------------------------------------------------
# Plotter dependencies
#----------------------------------------------------------------------------------------------------------------------

# Search for your dependencies here

#----------------------------------------------------------------------------------------------------------------------
# Plotter sources
#----------------------------------------------------------------------------------------------------------------------

include(GenerateExportHeader)
set(export_file_name "export_shared.h")

if(NOT BUILD_SHARED_LIBS)
    set(export_file_name "export_static.h")
endif()

generate_export_header(Plotter EXPORT_FILE_NAME include/Plotter/${export_file_name})

set(sources
    include/Plotter/export.h
    include/Plotter/Plotter.h
    src/Plotter.cpp
    src/glad.c)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})

#----------------------------------------------------------------------------------------------------------------------
# Plotter target
#----------------------------------------------------------------------------------------------------------------------

include(CMakePackageConfigHelpers)

target_sources(Plotter PRIVATE ${sources})
target_compile_definitions(Plotter PUBLIC "$<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:PLOTTER_STATIC_DEFINE>")

target_include_directories(Plotter
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>")

target_link_libraries(Plotter PUBLIC glfw dl GL)

set_target_properties(Plotter PROPERTIES
    SOVERSION ${PROJECT_VERSION_MAJOR}
    VERSION ${PROJECT_VERSION})

if(PLOTTER_INSTALL AND NOT CMAKE_SKIP_INSTALL_RULES)
    configure_package_config_file(cmake/plotter-config.cmake.in plotter-config.cmake
        INSTALL_DESTINATION "${PLOTTER_INSTALL_CMAKEDIR}")

    write_basic_package_version_file(plotter-config-version.cmake
        COMPATIBILITY SameMajorVersion)

    install(TARGETS Plotter EXPORT Plotter_export
        RUNTIME COMPONENT Plotter
        LIBRARY COMPONENT Plotter NAMELINK_COMPONENT Plotter-dev
        ARCHIVE COMPONENT Plotter-dev
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
    install(DIRECTORY include/
        TYPE INCLUDE
        COMPONENT Plotter-dev)
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/include/Plotter/${export_file_name}"
        COMPONENT Plotter-dev
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/Plotter")

    set(targets_file "Plotter-shared-targets.cmake")

    if(NOT BUILD_SHARED_LIBS)
        set(targets_file "Plotter-static-targets.cmake")
    endif()

    install(EXPORT Plotter_export
        COMPONENT Plotter-dev
        FILE "${targets_file}"
        DESTINATION "${PLOTTER_INSTALL_CMAKEDIR}"
        NAMESPACE Plotter::)

    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/Plotter-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/Plotter-config-version.cmake"
        COMPONENT Plotter-dev
        DESTINATION "${PLOTTER_INSTALL_CMAKEDIR}")

    if(MSVC)
        set(pdb_file "")
        set(pdb_file_destination "")

        if(BUILD_SHARED_LIBS)
            set(pdb_file "$<TARGET_PDB_FILE:Plotter>")
            set(pdb_file_destination "${CMAKE_INSTALL_BINDIR}")
        else()
            # TARGET_PDB_FILE does not work for pdb file generated for static library build, determining it manually
            set(pdb_file "$<TARGET_FILE_DIR:Plotter>/$<TARGET_FILE_PREFIX:Plotter>$<TARGET_FILE_BASE_NAME:Plotter>.pdb")
            set(pdb_file_destination "${CMAKE_INSTALL_LIBDIR}")
        endif()

        install(FILES "${pdb_file}"
            COMPONENT Plotter-dev
            CONFIGURATIONS Debug RelWithDebInfo
            DESTINATION "${pdb_file_destination}"
            OPTIONAL)
    endif()
endif()

#----------------------------------------------------------------------------------------------------------------------
# other targets
#----------------------------------------------------------------------------------------------------------------------

if(PLOTTER_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(PLOTTER_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(PLOTTER_BUILD_DOCS)
    find_package(Doxygen REQUIRED)
    doxygen_add_docs(docs include)
endif()

# include_directories(include libraries)

# add_subdirectory(src) 

# add_subdirectory(examples)
