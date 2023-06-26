cmake_minimum_required(VERSION 3.25)
project(test)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE test
        "*.h"
        "*.cc"
        "*.hpp")

add_executable(test main.cpp include/progress_bar.cc include/progress_bar.h include/progressbar.h)

target_include_directories(test PUBLIC include)
#./include/progressbar.hpp