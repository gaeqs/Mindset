# Copyright (c) 2025. VG-Lab/URJC.
#
# Authors: Gael Rial Costas <gael.rial.costas@urjc.es>
#
# This file is part of Mindset <https://gitlab.gmrv.es/g.rial/mindset>
#
# This library is free software; you can redistribute it and/or modify it under
# the terms of the GNU Lesser General Public License version 3.0 as published
# by the Free Software Foundation.
#
# This library is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
# details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this library; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

cmake_minimum_required(VERSION 3.16)
project(Mindset LANGUAGES CXX VERSION 1.0.2)

include(FetchContent)

# Configuration options
option(MINDSET_BUILD_SHARED "Build Mindset as a shared library" OFF)
option(MINDSET_USE_BRION "Import Brion" ON)
option(MINDSET_EXTERNAL_BRION "Use an external version of Brion" OFF)
option(MINDSET_TESTS "Include Mindset tests" ON)

# Global parameters
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(MINDSET_GENERATED_DIR ${CMAKE_BINARY_DIR}/generated)
set(MINDSET_SINGLE_HEADER_PATH ${CMAKE_BINARY_DIR}/generated/mindset/mindset.h)
set(EXTLIB_FROM_SUBMODULES ON) # For sonata

# Import Rush
FetchContent_Declare(
        rush
        GIT_REPOSITORY https://github.com/gaeqs/Rush.git
        GIT_TAG master
)

FetchContent_MakeAvailable(rush)
list(APPEND LIBRARIES rush)

# Import Hey
FetchContent_Declare(
        hey
        GIT_REPOSITORY https://github.com/gaeqs/Hey.git
        GIT_TAG master
)

FetchContent_MakeAvailable(hey)

# Import Brion
if (MINDSET_USE_BRION)
    if (NOT MINDSET_EXTERNAL_BRION)
        FetchContent_Declare(
                brion
                GIT_REPOSITORY https://github.com/gaeqs/Brion.git
                GIT_TAG master
        )
        FetchContent_MakeAvailable(brion)
    endif ()
    if (TARGET morphio_obj)
        target_precompile_headers(morphio_obj PUBLIC <cstdint>)
    endif ()

    list(APPEND LIBRARIES Brion Brain)
endif ()

if (NOT TARGET HighFive)
    FetchContent_Declare(
            HighFive
            GIT_REPOSITORY https://github.com/BlueBrain/HighFive.git
            GIT_TAG v2.7.1
    )

    FetchContent_MakeAvailable(HighFive)
else ()
    set(EMBEDDED_SONATA TRUE)
endif ()

find_package(pugixml REQUIRED)

list(APPEND LIBRARIES HighFive)
list(APPEND LIBRARIES hey)
list(APPEND LIBRARIES pugixml)

# Create version.h

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git")
    execute_process(
            COMMAND git rev-parse --short HEAD
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_COMMIT
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET
    )
else ()
    set(GIT_COMMIT "unknown")
endif ()

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.h.in
        ${MINDSET_GENERATED_DIR}/mindset/version.h
)

# Create the library
add_subdirectory(src)

if (MINDSET_TESTS)
    add_subdirectory(test)
endif ()

if (NOT EMBEDDED_SONATA)
    # Installation and export
    include(CMakePackageConfigHelpers)
    write_basic_package_version_file(
            "${CMAKE_CURRENT_BINARY_DIR}/MindsetConfigVersion.cmake"
            VERSION ${PROJECT_VERSION}
            COMPATIBILITY AnyNewerVersion
    )

    install(
            EXPORT MindsetTargets
            FILE MindsetTargets.cmake
            NAMESPACE Mindset::
            DESTINATION lib/cmake/Mindset
    )

    configure_package_config_file(
            "cmake/MindsetConfig.cmake.in"
            "${CMAKE_CURRENT_BINARY_DIR}/MindsetConfig.cmake"
            INSTALL_DESTINATION lib/cmake/Mindset
    )

    install(
            FILES
            "${CMAKE_CURRENT_BINARY_DIR}/MindsetConfig.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/MindsetConfigVersion.cmake"
            DESTINATION lib/cmake/Mindset
    )
endif ()

# Install public headers
install(
        DIRECTORY include/mindset
        DESTINATION include
)

install(
        DIRECTORY ${CMAKE_BINARY_DIR}/generated/mindset
        DESTINATION include
)