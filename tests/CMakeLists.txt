cmake_minimum_required(VERSION 3.16)
project(tests)
set(CMAKE_CXX_STANDARD 23)


add_subdirectory(graphtests)


include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG main
)
FetchContent_MakeAvailable(googletest)