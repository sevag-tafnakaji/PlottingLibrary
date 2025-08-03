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
    include/Plotter/data.h
    include/Plotter/line.h
    include/Plotter/scatter.h
    include/Plotter/resource_manager.h
    include/Plotter/shader.h
    include/Plotter/texture.h
    include/Plotter/stb_image.h
    include/Plotter/utils.h
    # include/freetype/config/ftconfig.h
    # include/freetype/config/ftheader.h
    # include/freetype/config/ftmodule.h
    # include/freetype/config/ftoption.h
    # include/freetype/config/ftstdlib.h
    # include/freetype/config/integer-types.h
    # include/freetype/config/mac-support.h
    # include/freetype/config/public-macros.h
    # include/freetype/internal/services/svbdf.h
    # include/freetype/internal/services/svcfftl.h
    # include/freetype/internal/services/svcid.h
    # include/freetype/internal/services/svfntfmt.h
    # include/freetype/internal/services/svgldict.h
    # include/freetype/internal/services/svgxval.h
    # include/freetype/internal/services/svkern.h
    # include/freetype/internal/services/svmetric.h
    # include/freetype/internal/services/svmm.h
    # include/freetype/internal/services/svotval.h
    # include/freetype/internal/services/svpfr.h
    # include/freetype/internal/services/svpostnm.h
    # include/freetype/internal/services/svprop.h
    # include/freetype/internal/services/svpscmap.h
    # include/freetype/internal/services/svpsinfo.h
    # include/freetype/internal/services/svsfnt.h
    # include/freetype/internal/services/svttcmap.h
    # include/freetype/internal/services/svtteng.h
    # include/freetype/internal/services/svttglyf.h
    # include/freetype/internal/services/svwinfnt.h
    # include/freetype/internal/autohint.h
    # include/freetype/internal/cffotypes.h
    # include/freetype/internal/cfftypes.h
    # include/freetype/internal/compiler-macros.h
    # include/freetype/internal/ftcalc.h
    # include/freetype/internal/ftdebug.h
    # include/freetype/internal/ftdrv.h
    # include/freetype/internal/ftgloadr.h
    # include/freetype/internal/fthash.h
    # include/freetype/internal/ftmemory.h
    # include/freetype/internal/ftmmtypes.h
    # include/freetype/internal/ftobjs.h
    # include/freetype/internal/ftpsprop.h
    # include/freetype/internal/ftrfork.h
    # include/freetype/internal/ftserv.h
    # include/freetype/internal/ftstream.h
    # include/freetype/internal/fttrace.h
    # include/freetype/internal/ftvalid.h
    # include/freetype/internal/pshints.h
    # include/freetype/internal/sfnt.h
    # include/freetype/internal/svginterface.h
    # include/freetype/internal/t1types.h
    # include/freetype/internal/tttypes.h
    # include/freetype/internal/wofftypes.h
    # include/freetype/freetype.h
    # include/freetype/ftadvanc.h
    # include/freetype/ftbbox.h
    # include/freetype/ftbdf.h
    # include/freetype/ftbitmap.h
    # include/freetype/ftbzip2.h
    # include/freetype/ftcache.h
    # include/freetype/ftchapters.h
    # include/freetype/ftcid.h
    # include/freetype/ftcolor.h
    # include/freetype/ftdriver.h
    # include/freetype/fterrdef.h
    # include/freetype/fterrors.h
    # include/freetype/ftfntfmt.h
    # include/freetype/ftgasp.h
    # include/freetype/ftglyph.h
    # include/freetype/ftgxval.h
    # include/freetype/ftgzip.h
    # include/freetype/ftimage.h
    # include/freetype/ftincrem.h
    # include/freetype/ftlcdfil.h
    # include/freetype/ftlist.h
    # include/freetype/ftlogging.h
    # include/freetype/ftlzw.h
    # include/freetype/ftmac.h
    # include/freetype/ftmm.h
    # include/freetype/ftmodapi.h
    # include/freetype/ftmoderr.h
    # include/freetype/ftotval.h
    # include/freetype/ftoutln.h
    # include/freetype/ftparams.h
    # include/freetype/ftpfr.h
    # include/freetype/ftrender.h
    # include/freetype/ftsizes.h
    # include/freetype/ftsnames.h
    # include/freetype/ftstroke.h
    # include/freetype/ftsynth.h
    # include/freetype/ftsystem.h
    # include/freetype/fttrigon.h
    # include/freetype/fttypes.h
    # include/freetype/ftwinfnt.h
    # include/freetype/otsvg.h
    # include/freetype/t1tables.h
    # include/freetype/ttnameid.h
    # include/freetype/tttables.h
    # include/freetype/tttags.h
    # include/ft2build.h
    src/Plotter.cpp
    src/data.cpp
    src/line.cpp
    src/scatter.cpp
    src/resource_manager.cpp
    src/shader.cpp
    src/texture.cpp
    src/stb_image.cpp
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

target_link_libraries(Plotter PUBLIC glfw dl GL freetype)

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