cmake_minimum_required(VERSION 3.10)
project(telemetry_project)

set(CMAKE_CXX_STANDARD 17)

# Настройка Boost
find_package(Boost 1.66 REQUIRED COMPONENTS system)

# Просто скачиваем json.hpp в текущую директорию
file(DOWNLOAD 
    https://github.com/nlohmann/json/releases/download/v3.11.2/json.hpp
    ${CMAKE_BINARY_DIR}/json.hpp
)

# Добавляем путь к скачанному файлу
include_directories(${CMAKE_BINARY_DIR})
enable_testing()

add_subdirectory(common)
add_subdirectory(server)
add_subdirectory(client)
add_subdirectory(tests)

