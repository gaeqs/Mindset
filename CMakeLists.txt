cmake_minimum_required(VERSION 3.16)
project(Mnemea LANGUAGES CXX VERSION 1.2.3)

include(FetchContent)

# Configuration options
option(MNEMEA_BUILD_SHARED "Build Mnemea as a shared library" OFF)
option(MNEMEA_USE_BRION "Import Brion" ON)

# Global parameters
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Import Brion
if(MNEMEA_USE_BRION)
    FetchContent_Declare(
            brion
            GIT_REPOSITORY https://github.com/BlueBrain/Brion.git
            GIT_TAG master
            SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/brion
    )

    FetchContent_MakeAvailable(brion)
    list(APPEND LIBRARIES Brion)
endif ()

# Create the library
add_subdirectory(src)

# Installation and export
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/MnemeaConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)

install(
        EXPORT MnemeaTargets
        FILE MnemeaTargets.cmake
        NAMESPACE Mnemea::
        DESTINATION lib/cmake/Mnemea
)

configure_package_config_file(
        "cmake/MnemeaConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/MnemeaConfig.cmake"
        INSTALL_DESTINATION lib/cmake/Mnemea
)

install(
        FILES
        "${CMAKE_CURRENT_BINARY_DIR}/MnemeaConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/MnemeaConfigVersion.cmake"
        DESTINATION lib/cmake/Mnemea
)

# Install public headers
install(
        DIRECTORY include/Mnemea
        DESTINATION include
)