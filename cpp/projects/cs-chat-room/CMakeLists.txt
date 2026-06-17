cmake_minimum_required(VERSION 3.20)

project(CsChatRoom LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)

add_executable(chat_server
    backend/src/server.cpp
    backend/src/AsyncTcpServer.cpp
    backend/src/SocketUtils.cpp
    backend/src/WebSocket.cpp
    backend/src/ClientManager.cpp
)

target_include_directories(chat_server PRIVATE
    ${PROJECT_SOURCE_DIR}/backend/include
)

target_compile_features(chat_server PRIVATE cxx_std_23)

target_link_libraries(chat_server PRIVATE
    Threads::Threads
    OpenSSL::Crypto
    spdlog::spdlog
    fmt::fmt
)

add_executable(websocket_test
    backend/test/websocket_test.cpp
    backend/src/WebSocket.cpp
)

target_include_directories(websocket_test PRIVATE
    ${PROJECT_SOURCE_DIR}/backend/include
)

target_compile_features(websocket_test PRIVATE cxx_std_23)

target_link_libraries(websocket_test PRIVATE
    OpenSSL::Crypto
)
