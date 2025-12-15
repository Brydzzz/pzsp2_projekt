include_directories("${csvreader_SOURCE_DIR}")

add_executable(graphtest GraphTests.cpp
        ../../src/graph/Graph.tpp
)

target_include_directories(graphtest PRIVATE ../../src/graph)
target_link_libraries(graphtest gtest gtest_main)

add_executable(graphParserTest GraphParserTests.cpp
        ../../src/graph/GraphParser.h ../../src/graph/GraphParser.cpp
        ../../src/graph/Graph.h ../../src/graph/Graph.tpp
        ../../src/graph/NetStat.h
        ../../src/graph/Edge.h
        ../../src/graph/Node.h)

target_include_directories(graphParserTest PRIVATE ../../src/graph ${csvReader_SOURCE_DIR})
target_link_libraries(graphParserTest gtest gtest_main)



include(GoogleTest)
gtest_discover_tests(graphtest)
gtest_discover_tests(graphParserTest)