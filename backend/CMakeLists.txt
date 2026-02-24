cmake_minimum_required(VERSION 3.15)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
project(FitnessAppBackend)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include modules for fetching content
include(FetchContent)

# Fetch Asio
FetchContent_Declare(
  asio
  GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
  GIT_TAG asio-1-28-0
)
FetchContent_MakeAvailable(asio)
set(ASIO_INCLUDE_DIR ${asio_SOURCE_DIR}/asio/include CACHE PATH "Path to Asio headers")

# Fetch Crow
FetchContent_Declare(
  crow
  GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
  GIT_TAG master
)
FetchContent_MakeAvailable(crow)

# Fetch JSON library
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(json)

add_executable(fitness_server main.cpp)

# Asio is header-only, so we just need the include path and system libraries on Windows
target_include_directories(fitness_server PRIVATE ${asio_SOURCE_DIR}/asio/include)

# Standard Libraries
target_link_libraries(fitness_server PRIVATE Crow::Crow nlohmann_json::nlohmann_json)

# On Windows, Asio needs ws2_32 and mswsock for networking
if(WIN32)
    target_link_libraries(fitness_server PRIVATE ws2_32 mswsock)
endif()
