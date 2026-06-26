cmake_minimum_required(VERSION 3.20)

project(LexicalAnalyzer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(lexer
    src/main.cpp
    src/Lexer.cpp
    src/Preprocessor.cpp
    src/SymbolTable.cpp
)

target_include_directories(lexer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
