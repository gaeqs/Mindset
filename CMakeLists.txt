cmake_minimum_required(VERSION 3.16)
project(Mnemea LANGUAGES CXX VERSION 1.2.3)

include(FetchContent)

# Configuration options
option(MNEMEA_BUILD_SHARED "Build Mnemea as a shared library" OFF)
option(MNEMEA_USE_BRION "Import Brion" ON)
option(MNEMEA_EXTERNAL_BRION "Use an external version of Brion" OFF)
option(MNEMEA_TESTS "Include Mnemea tests" ON)

# Global parameters
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Import Rush
FetchContent_Declare(
        rush
        GIT_REPOSITORY https://github.com/gaeqs/Rush.git
        GIT_TAG master
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/rush
)

FetchContent_MakeAvailable(rush)
list(APPEND LIBRARIES rush)

# Import Hey
FetchContent_Declare(
        hey
        GIT_REPOSITORY https://github.com/gaeqs/Hey.git
        GIT_TAG master
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/hey
)

FetchContent_MakeAvailable(hey)
list(APPEND LIBRARIES hey)

# Import Brion
if (MNEMEA_USE_BRION)
    if (NOT MNEMEA_EXTERNAL_BRION)
        FetchContent_Declare(
                brion
                GIT_REPOSITORY https://github.com/BlueBrain/Brion.git
                GIT_TAG master
                SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/brion
        )
        FetchContent_MakeAvailable(brion)
    endif ()
    list(APPEND LIBRARIES Brion Brain)
else ()
    # Import HighFive. Do not import if Brion is used.
    FetchContent_Declare(
            highfive
            GIT_REPOSITORY https://github.com/BlueBrain/HighFive.git
            GIT_TAG master
            SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/highfive
    )

    FetchContent_MakeAvailable(highfive)
endif ()
list(APPEND LIBRARIES HighFive)

# Import pugiXML
find_package(pugixml REQUIRED)
list(APPEND LIBRARIES pugixml)

# Create the library
add_subdirectory(src)

if (MNEMEA_TESTS)
    add_subdirectory(test)
endif ()

# Installation and export
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/MnemeaConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)

if (NOT MNEMEA_USE_BRION)
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
endif ()