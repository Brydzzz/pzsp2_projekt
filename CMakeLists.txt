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


add_executable(gen_intents
    ./src/cli_tools/gen_intents.cpp
    ${COMMON_SOURCES}
)
target_include_directories(gen_intents PRIVATE ${COMMON_INCLUDES})

add_executable(gen_true_pf
    ./src/cli_tools/gen_true_pf.cpp
    ${COMMON_SOURCES}
)
target_include_directories(gen_true_pf PRIVATE ${COMMON_INCLUDES})