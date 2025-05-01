cmake_minimum_required(VERSION 3.16)
project(Mindset LANGUAGES CXX VERSION 0.0.1)

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
list(APPEND LIBRARIES hey)

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
else ()
    # Import HighFive. Do not import if Brion is used.
    FetchContent_Declare(
            highfive
            GIT_REPOSITORY https://github.com/BlueBrain/HighFive.git
            GIT_TAG master
    )

    FetchContent_MakeAvailable(highfive)
endif ()
list(APPEND LIBRARIES HighFive)

# Import pugiXML
find_package(pugixml REQUIRED)
list(APPEND LIBRARIES pugixml)

# Create version.h

if (EXISTS "${CMAKE_SOURCE_DIR}/.git")
    execute_process(
            COMMAND git rev-parse --short HEAD
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
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

# Installation and export
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/MindsetConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)

if (NOT MINDSET_USE_BRION)
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

    # Install public headers
    install(
            DIRECTORY include/mindset
            DESTINATION include
    )

    install(
            DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/generated/mindset
            DESTINATION include
    )
endif ()