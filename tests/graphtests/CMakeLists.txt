add_executable(graphtest GraphTests.cpp
        ../../src/graph/Graph.tpp
)

target_include_directories(graphtest PRIVATE ../../src/graph)

target_link_libraries(graphtest gtest gtest_main)

include(GoogleTest)
gtest_discover_tests(graphtest)