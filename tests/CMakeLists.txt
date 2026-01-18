cmake_minimum_required(VERSION 3.16)
project(tests)

set(CMAKE_CXX_STANDARD 23)

# 1. Define a 'coverage_options' interface library.
# This acts as a "folder" for our coverage settings that we can plug into our tests.
add_library(coverage_options INTERFACE)

# 2. Apply flags to the interface. 
# -O0 is vital; without it, the compiler optimizes away lines, ruining the report.
target_compile_options(coverage_options INTERFACE 
    --coverage 
    -O0 
    -g 
    -Wall 
    -Werror
    -fno-inline     # Prevent function inlining
    -fno-elide-constructors # Prevent certain C++ optimizations
)

target_link_options(coverage_options INTERFACE --coverage)

# --- Dependencies ---
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

# --- Subdirectories ---
add_subdirectory(graphtests)
add_subdirectory(intenttests)
add_subdirectory(spea2tests)
add_subdirectory(nsga2tests)