find_package(Catch2 REQUIRED)

include(CTest)
include(Catch)

project(mindset-tests)
set(CMAKE_CXX_STANDARD 20)

add_executable(mindset-tests brion.cpp swc.cpp)

add_dependencies(mindset-tests mindset)

target_link_libraries(mindset-tests PRIVATE mindset Catch2::Catch2WithMain)

catch_discover_tests(mindset-tests)

add_custom_command(
        TARGET mindset-tests PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/data
        ${CMAKE_CURRENT_BINARY_DIR}/data
)