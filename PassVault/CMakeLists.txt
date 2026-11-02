cmake_minimum_required(VERSION 3.20)
project(passvault LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SODIUM REQUIRED IMPORTED_TARGET libsodium)

set(PASSVAULT_SOURCES
    src/main.cpp
    src/generator.hpp
    src/generator.cpp
)

add_executable(passvault ${PASSVAULT_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "src" FILES ${PASSVAULT_SOURCES})

target_link_libraries(passvault PRIVATE PkgConfig::SODIUM)
target_compile_options(passvault PRIVATE -Wall -Wextra -Wpedantic)

set_target_properties(passvault PROPERTIES
    XCODE_GENERATE_SCHEME ON
    XCODE_SCHEME_ARGUMENTS "help"
    XCODE_SCHEME_ADDRESS_SANITIZER ON
    XCODE_SCHEME_UNDEFINED_BEHAVIOUR_SANITIZER ON
)

add_custom_target(project_files SOURCES
    CMakeLists.txt
    CMakePresets.json
    Brewfile
    .gitignore
    .clang-format
)
