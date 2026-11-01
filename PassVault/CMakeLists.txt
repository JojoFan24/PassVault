cCmake_minimum_required(VERSION 3.20)
project(passvault LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SODIUM REQUIRED IMPORTED_TARGET libsodium)

add_executable(passvault
    src/main.cpp
    src/generator.cpp
)

target_link_libraries(passvault PRIVATE PkgConfig::SODIUM)
target_compile_options(passvault PRIVATE -Wall -Wextra -Wpedantic)
