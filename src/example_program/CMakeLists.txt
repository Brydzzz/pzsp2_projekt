cmake_minimum_required(VERSION 3.16)
project(example_program)
set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

add_compile_options(-g -Wall -Werror)

FetchContent_Declare(
        csvReader
        GIT_REPOSITORY https://github.com/ben-strasser/fast-cpp-csv-parser.git
        GIT_TAG master
)
FetchContent_MakeAvailable(csvReader)

FetchContent_Declare(pareto
        GIT_REPOSITORY https://github.com/alandefreitas/pareto
        GIT_TAG origin/master
)
FetchContent_MakeAvailable(pareto)

add_executable(exampleProgram example_program.cpp ../../src/intent/Intent.cpp ../../src/graph/GraphParser.cpp ../../src/graph/Graph.tpp)

target_link_libraries(exampleProgram PRIVATE pareto)

include_directories(${csvreader_SOURCE_DIR})

target_include_directories(exampleProgram PRIVATE ../../src/graph ../../src/intent ../../src/algorithms ${csvreader_SOURCE_DIR})

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/full_mesh_10.csv"
        DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

add_executable(insga_example_program insga_example_program.cpp ../../src/intent/Intent.cpp ../../src/graph/GraphParser.cpp ../../src/graph/Individual.cpp ../../src/algorithms/MutationOperators.cpp ../../src/graph/Graph.tpp)

target_link_libraries(insga_example_program PRIVATE pareto)

target_include_directories(insga_example_program PRIVATE ../../src/graph ../../src/intent ../../src/algorithms ${csvreader_SOURCE_DIR})
