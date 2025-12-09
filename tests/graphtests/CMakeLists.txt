add_executable(graphtest GraphTests.cpp
        ../../src/graph/Graph.h ../../src/graph/Graph.tpp
        ../../src/graph/Edge.h
        ../../src/graph/Node.h)

target_include_directories(graphtest PRIVATE ../../src/graph)

target_link_libraries(graphtest gtest gtest_main)

include(GoogleTest)
gtest_discover_tests(graphtest)