cmake_minimum_required(VERSION 3.20)
project(museflow VERSION 0.4.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
find_package(Threads REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(CURL REQUIRED)
find_package(nlohmann_json 3.10 REQUIRED)
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/schema/music-blueprint.schema.json" MUSEFLOW_BLUEPRINT_SCHEMA)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/schema/music-blueprint.schema.json")
configure_file(schema/embedded_schema.hpp.in generated/embedded_schema.hpp @ONLY)
if(TARGET SQLite3::SQLite3)
  set(MUSEFLOW_SQLITE_TARGET SQLite3::SQLite3)
else()
  set(MUSEFLOW_SQLITE_TARGET SQLite::SQLite3)
endif()
add_library(museflow_core
  src/models/blueprint.cpp
  src/models/schema.cpp
  src/utils/runtime.cpp
  src/utils/http_client.cpp
  src/providers/mock.cpp
  src/providers/deepseek.cpp
  src/providers/lyria.cpp
  src/providers/songapi.cpp
  src/providers/elevenlabs.cpp
  src/services/audio_processing.cpp
  src/repositories/database.cpp
  src/services/generation_queue.cpp
  src/services/settings.cpp
  src/services/text_jobs.cpp)
target_include_directories(museflow_core PUBLIC include)
target_include_directories(museflow_core PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/generated")
target_link_libraries(museflow_core PUBLIC nlohmann_json::nlohmann_json ${MUSEFLOW_SQLITE_TARGET} Threads::Threads CURL::libcurl)
target_compile_options(museflow_core PRIVATE -Wall -Wextra -Wpedantic)
add_executable(museflow src/main.cpp src/api/server.cpp)
target_include_directories(museflow SYSTEM PRIVATE third_party)
target_link_libraries(museflow PRIVATE museflow_core)
target_compile_options(museflow PRIVATE -Wall -Wextra -Wpedantic)
target_compile_definitions(museflow PRIVATE
  MUSEFLOW_ASSET_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../assets"
  MUSEFLOW_WEB_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../frontend/dist")
include(CTest)
if(BUILD_TESTING)
  add_executable(museflow_tests tests/core_tests.cpp)
  target_link_libraries(museflow_tests PRIVATE museflow_core)
  target_compile_definitions(museflow_tests PRIVATE MUSEFLOW_ASSET_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../assets")
  add_test(NAME core COMMAND museflow_tests)
endif()
option(MUSEFLOW_BUILD_DESKTOP "Build the Qt desktop application" ON)
if(MUSEFLOW_BUILD_DESKTOP)
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../desktop" "${CMAKE_CURRENT_BINARY_DIR}/desktop")
endif()
