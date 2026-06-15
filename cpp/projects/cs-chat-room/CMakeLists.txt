cmake_minimum_required(VERSION 3.20)

project(CsChatRoom LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Threads REQUIRED)

add_executable(chat_server
    backend/src/server.cpp
    backend/src/SocketUtils.cpp
    backend/src/WebSocket.cpp
    backend/src/ClientManager.cpp
)

target_include_directories(chat_server PRIVATE
    ${PROJECT_SOURCE_DIR}/backend/include
)

target_compile_features(chat_server PRIVATE cxx_std_23)

target_link_libraries(chat_server PRIVATE Threads::Threads)

set_target_properties(chat_server PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/backend/bin
)
