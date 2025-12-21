cmake_minimum_required(VERSION 3.16)
project(tests)
set(CMAKE_CXX_STANDARD 23)

add_compile_options(-g -Wall -Werror)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG main
)

FetchContent_Declare(
        csvReader
        GIT_REPOSITORY https://github.com/ben-strasser/fast-cpp-csv-parser.git
        GIT_TAG master
)
FetchContent_MakeAvailable(googletest csvReader)

enable_testing()

add_subdirectory(graphtests)
add_subdirectory(intenttests)
add_subdirectory(spea2tests)

