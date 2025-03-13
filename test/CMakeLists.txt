find_package(Catch2 REQUIRED)

include(CTest)
include(Catch)

project(mnemea-tests)
set(CMAKE_CXX_STANDARD 20)

add_executable(mnemea-tests brion.cpp)

target_link_libraries(mnemea-tests PUBLIC ${LIBRARIES} mnemea Catch2::Catch2WithMain)

catch_discover_tests(mnemea-tests)