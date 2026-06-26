cmake_minimum_required(VERSION 3.20)

project(parser LANGUAGES CXX)

add_executable(parser
    src/main.cpp
    src/Scanner.cpp
    src/Parser.cpp
    src/Reduction.cpp
)

target_include_directories(parser PRIVATE include)

target_compile_features(parser PRIVATE cxx_std_23)
