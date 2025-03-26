find_package(Catch2 REQUIRED)

include(CTest)
include(Catch)

project(mindset-tests)
set(CMAKE_CXX_STANDARD 20)

add_executable(mindset-tests brion.cpp)

target_link_libraries(mindset-tests PUBLIC ${LIBRARIES} mindset Catch2::Catch2WithMain)

catch_discover_tests(mindset-tests)