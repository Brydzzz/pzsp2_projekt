add_executable(nsga2test Nsga2Tests.cpp
        ../../src/algorithms/Evolutionary_Algorithm.h
        ../../src/algorithms/NSGA2.h)


target_include_directories(nsga2test PRIVATE ../../src/algorithms)

include(FetchContent)

FetchContent_Declare(pareto
        GIT_REPOSITORY https://github.com/alandefreitas/pareto
        GIT_TAG origin/master # or whatever tag you want
)

FetchContent_GetProperties(pareto)
if (NOT pareto_POPULATED)
    FetchContent_Populate(pareto)
    add_subdirectory(${pareto_SOURCE_DIR} ${pareto_BINARY_DIR} EXCLUDE_FROM_ALL)
endif ()

# ...
target_link_libraries(nsga2test PRIVATE pareto gtest gtest_main)

include(GoogleTest)
gtest_discover_tests(nsga2test)