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

add_executable(exampleProgram example_program.cpp ../../src/intent/Intent.cpp ../../src/graph/GraphParser.cpp
        ../../src/graph/Graph.h ../../src/graph/Graph.tpp)

include_directories(${csvreader_SOURCE_DIR})

target_include_directories(exampleProgram PRIVATE ../../src/graph ../../src/intent ${csvreader_SOURCE_DIR})

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/full_mesh_10.csv"
        DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")