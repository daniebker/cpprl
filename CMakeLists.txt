cmake_minimum_required (VERSION 3.13...3.21)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE
        "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
endif()

project(
    roguelike
    LANGUAGES C CXX
)

set(CMAKE_CXX_ANALYZE_ON_COMPILE True)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")  # Keep all runtime files in one directory.

file(
    GLOB_RECURSE SOURCE_FILES
    CONFIGURE_DEPENDS  # Automatically reconfigure if source files are added/removed.
    ${PROJECT_SOURCE_DIR}/app/src/*.cpp
    ${PROJECT_SOURCE_DIR}/app/src/*.hpp
    ${PROJECT_SOURCE_DIR}/app/src/*.h
)

add_subdirectory(lib)
add_subdirectory(app)
add_subdirectory(tests)
