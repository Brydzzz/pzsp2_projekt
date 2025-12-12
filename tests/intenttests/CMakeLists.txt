add_executable(intenttests IntentTests.cpp
        ../../src/graph/Graph.tpp
        ../../src/intent/Intent.cpp
)

target_include_directories(intenttests PRIVATE ../../src/intent)

target_link_libraries(intenttests gtest gtest_main)

include(GoogleTest)
gtest_discover_tests(intenttests)