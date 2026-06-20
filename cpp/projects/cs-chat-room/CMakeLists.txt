cmake_minimum_required(VERSION 3.20)

project(CsChatRoom LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(spdlog REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(chat_server
    backend/src/server.cpp
    backend/src/AsyncTcpServer.cpp
    backend/src/WebSocket.cpp
    backend/src/WebSocketServer.cpp
    backend/src/ChatServer.cpp
    backend/src/Database.cpp
    backend/src/UserRepository.cpp
    backend/src/PasswordHasher.cpp
)

target_include_directories(chat_server PRIVATE
    ${PROJECT_SOURCE_DIR}/backend/include
    ${SQLite3_INCLUDE_DIRS}
)

target_compile_features(chat_server PRIVATE cxx_std_23)

target_link_libraries(chat_server PRIVATE
    Threads::Threads
    OpenSSL::Crypto
    spdlog::spdlog
    ${SQLite3_LIBRARIES}
    nlohmann_json::nlohmann_json
)

set_target_properties(chat_server PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}
)
