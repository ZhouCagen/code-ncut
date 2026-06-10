cmake_minimum_required(VERSION 3.20)

project(SocketLab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(spdlog REQUIRED)

add_executable(udpServer src/udpServer.cpp)
add_executable(udpClient src/udpClient.cpp)

target_link_libraries(udpServer PRIVATE spdlog::spdlog)
target_link_libraries(udpClient PRIVATE spdlog::spdlog)
