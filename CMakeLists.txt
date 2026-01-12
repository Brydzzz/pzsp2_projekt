cmake_minimum_required(VERSION 3.16)
project(CliTools)
set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

add_compile_options(-g -Wall -Werror)

FetchContent_Declare(
        csvReader
        GIT_REPOSITORY https://github.com/ben-strasser/fast-cpp-csv-parser.git
        GIT_TAG master
)
FetchContent_MakeAvailable(csvReader)


set(COMMON_SOURCES
    ./src/intent/Intent.cpp
    ./src/graph/GraphParser.cpp
    ./src/graph/Graph.tpp
)


set(COMMON_INCLUDES
    ./src/graph
    ./src/intent
    ${csvreader_SOURCE_DIR}
)

include_directories(${csvreader_SOURCE_DIR})


add_executable(gen_intents
    ./src/cli_tools/gen_intents.cpp
    ${COMMON_SOURCES}
    ./src/graph/Graph.tpp
)
target_include_directories(gen_intents PRIVATE ${COMMON_INCLUDES})