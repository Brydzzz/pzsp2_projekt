add_executable(graphtest GraphTests.cpp
        ../../src/graph/Graph.h)

target_link_libraries(graphtest gtest gtest_main)

include(GoogleTest)
gtest_discover_tests(graphtest)