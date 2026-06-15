cmake_minimum_required(VERSION 3.20)

project(SocketLab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(spdlog REQUIRED)
find_package(Threads REQUIRED)

add_executable(udpServer src/udp/udpServer.cpp)
target_link_libraries(udpServer PRIVATE spdlog::spdlog Threads::Threads)

add_executable(udpClient src/udp/udpClient.cpp)
target_link_libraries(udpClient PRIVATE spdlog::spdlog Threads::Threads)

add_executable(tcpServer src/tcp/tcpServer.cpp)
target_link_libraries(tcpServer PRIVATE spdlog::spdlog Threads::Threads)

add_executable(tcpClient src/tcp/tcpClient.cpp)
target_link_libraries(tcpClient PRIVATE spdlog::spdlog Threads::Threads)
