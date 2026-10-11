
cmake_minimum_required(VERSION 3.20)

project(TodoApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PQXX REQUIRED IMPORTED_TARGET libpqxx)

add_executable(todo main.cpp)

target_link_libraries(todo PRIVATE PkgConfig::PQXX)
